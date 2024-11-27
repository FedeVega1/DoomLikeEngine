namespace LevelEditor
{
    readonly internal struct GridEditorData
    {
        public readonly ToolStripStatusLabel lblCursor, lblOrigin, lblGridSize;
        public readonly ToolStripButton btnWallTopColor, btnWallInColor, btnWallBtmColor;
        public readonly ToolStripButton btnCeillingColor, btnFloorColor;
        public readonly ToolStripNumberControl numbCeilling, numbFloor;
        public readonly PictureBox imgEditorDraw;
        public readonly ToolStripLabel lblSelectionData;

        public GridEditorData(ref ToolStripStatusLabel cursor, ref ToolStripStatusLabel origin, ref ToolStripStatusLabel gridSize, 
            ref PictureBox editorDraw, ref ToolStripButton wallTopColor, ref ToolStripButton wallInColor, ref ToolStripButton wallBtmColor, 
            ref ToolStripButton ceilColor, ref ToolStripButton floorColor, ref ToolStripNumberControl ceilNumb, 
            ref ToolStripNumberControl floorNumb, ref ToolStripLabel selectData)
        {
            lblCursor = cursor;
            lblOrigin = origin;
            lblGridSize = gridSize;
            imgEditorDraw = editorDraw;
            btnWallTopColor = wallTopColor;
            btnWallInColor = wallInColor;
            btnWallBtmColor = wallBtmColor;
            btnCeillingColor = ceilColor;
            btnFloorColor = floorColor;
            numbCeilling = ceilNumb;
            numbFloor = floorNumb;
            lblSelectionData = selectData;
        }
    }

    internal class MapCursor
    {
        System.Windows.Forms.Timer moveMapTimer;
        Action<object, EventArgs> timerTick;

        public bool CursorIsActive { get; private set; }
        public Point MouseCurrentPos { get; set; }

        public MapCursor(Action<object, EventArgs> tick)
        {
            moveMapTimer = new System.Windows.Forms.Timer();
            timerTick = tick;
        }

        ~MapCursor()
        {
            moveMapTimer.Stop();
        }

        public void ToggleCursor(bool toggle)
        {
            if (toggle)
            {
                if (CursorIsActive) return;
                Cursor.Hide();
                moveMapTimer.Interval = 50;
                moveMapTimer.Tick += MoveMapTimer_Tick;
                moveMapTimer.Start();
                CursorIsActive = true;
                return;
            }

            if (!CursorIsActive) return;
            Cursor.Show();
            moveMapTimer.Stop();
            CursorIsActive = false;
        }

        void MoveMapTimer_Tick(object sender, EventArgs e) => timerTick?.Invoke(sender, e);

        public void DrawCursor(ref Graphics graph)
        {
            if (!CursorIsActive) return;
            graph.DrawLine(Pens.Cyan, new Point(MouseCurrentPos.X, MouseCurrentPos.Y - 10), new Point(MouseCurrentPos.X, MouseCurrentPos.Y + 10));
            graph.DrawLine(Pens.Cyan, new Point(MouseCurrentPos.X - 10, MouseCurrentPos.Y), new Point(MouseCurrentPos.X + 10, MouseCurrentPos.Y));
        }
    }

    internal enum EditorMode { None, LineMode, NodeMode, WallMode, SectorMode }

    internal class MapGridEditor
    {
        bool isDraggingPanel;
        EditorMode currentMode;
        Point lastMousePos;

        public bool ShiftKeyDown { get; set; }
        public bool CtrlKeyDown { get; set; }

        readonly GridEditorData refData;
        readonly Grid grid;
        readonly MapCursor cursor;
        readonly SectorDrawer sectorDrawer;
        readonly SelectionManager selectionManager;

        public bool IsDrawingLine => sectorDrawer.IsDrawingLine;

        public MapGridEditor(GridEditorData data)
        {
            refData = data;

            grid = new Grid(refData.imgEditorDraw.Right, refData.imgEditorDraw.Bottom);
            cursor = new MapCursor(MoveMapTimer_Tick);
            sectorDrawer = new SectorDrawer(this);
            selectionManager = new SelectionManager(ref sectorDrawer, ref grid);
            selectionManager.OnSelection += OnSelection;
            selectionManager.OnDeselect += OnDeselect;

            refData.lblCursor.Visible = false;
            UpdateOriginPosText();
            UpdateGridSizeText();
        }

        #region Events

        public void OnPaint(ref Graphics graph)
        {
            grid.DrawGrid(ref graph);

            switch (currentMode)
            {
                case EditorMode.LineMode:
                    sectorDrawer.OnLineModeDraw(ref graph);
                    cursor.DrawCursor(ref graph);
                    sectorDrawer.DrawSector(ref graph);
                    break;

                case EditorMode.NodeMode:
                    sectorDrawer.DrawSector(ref graph, new SectorDrawingData { showWallNodes = true });
                    break;

                case EditorMode.WallMode:
                    sectorDrawer.DrawSector(ref graph, new SectorDrawingData { showWallNodes = false });
                    break;

                case EditorMode.SectorMode:
                    sectorDrawer.DrawSector(ref graph, new SectorDrawingData { showWallNodes = false });
                    break;

                default:
                    sectorDrawer.DrawSector(ref graph);
                    break;
            }

            //graph.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
            //graph.DrawString(currentMode.ToString(), displayFont, displaytextBrush, graph.VisibleClipBounds.Right - 100, graph.VisibleClipBounds.Bottom - 32);
        }

        public void OnMouseDown(MouseButtons button, Point location)
        {
            if (button != MouseButtons.Left) return;

            switch (currentMode)
            {
                case EditorMode.LineMode:
                    if (sectorDrawer.IsNotDrawingLine)
                    {
                        sectorDrawer.ToggleLineDrawMode(true, cursor.MouseCurrentPos);
                        return;
                    }

                    if (sectorDrawer.OnMouseDown(cursor.MouseCurrentPos))
                        refData.imgEditorDraw.Invalidate();
                    return;

                case EditorMode.NodeMode:
                case EditorMode.WallMode:
                case EditorMode.SectorMode:
                    selectionManager.OnMouseDown(location, ShiftKeyDown, CtrlKeyDown);
                    if (selectionManager.HasSelection) return;
                    break;
            }

            isDraggingPanel = true;
            lastMousePos = location;
            refData.lblCursor.Visible = false;
        }

        public void OnMouseMove(Point location)
        {
            switch (currentMode)
            {
                case EditorMode.LineMode:
                    cursor.MouseCurrentPos = grid.ParseMousePosToGridPos(location);
                    sectorDrawer.SetLineEnd(cursor.MouseCurrentPos);
                    refData.imgEditorDraw.Invalidate();
                    break;

                case EditorMode.NodeMode:
                case EditorMode.WallMode:
                case EditorMode.SectorMode:
                    sectorDrawer.GetMousePos(location);
                    selectionManager.OnMouseMove(grid.ParseMousePosToGridPos(location));
                    refData.imgEditorDraw.Invalidate();
                    lastMousePos = location;
                    break;
            }

            UpdateCursorPosText(location);

            if (!isDraggingPanel) return;

            MoveOrigin(GetMouseMovDelta(location));
            lastMousePos = location;
        }

        public void OnMouseUp(MouseButtons button)
        {
            if (button != MouseButtons.Left) return;

            switch (currentMode)
            {
                case EditorMode.NodeMode:
                case EditorMode.WallMode:
                case EditorMode.SectorMode:
                    selectionManager.OnMouseUp();
                    break;
            }

            isDraggingPanel = false;
            refData.lblCursor.Visible = true;
        }

        public void OnMouseEnter()
        {
            switch (currentMode)
            {
                case EditorMode.LineMode:
                    cursor.ToggleCursor(true);
                    break;

                case EditorMode.NodeMode:
                case EditorMode.WallMode:
                case EditorMode.SectorMode:
                    sectorDrawer.OnMouseEnter();
                    break;
            }

            if (isDraggingPanel) return;
            refData.lblCursor.Visible = true;
        }

        public void OnMouseLeave()
        {
            switch (currentMode)
            {
                case EditorMode.LineMode:
                    refData.lblCursor.Visible = false;
                    break;

                case EditorMode.NodeMode:
                case EditorMode.WallMode:
                case EditorMode.SectorMode:
                    sectorDrawer.OnMouseLeave();
                    break;
            }
        }

        #endregion

        public void UpdateGridSize(int sign)
        {
            if (grid.UpdateGridSize(sign)) refData.imgEditorDraw.Invalidate();
            cursor.MouseCurrentPos = grid.ParseMousePosToGridPos(cursor.MouseCurrentPos);
            if (!sectorDrawer.DrawLineStart.IsEmpty) sectorDrawer.DrawLineStart = grid.ParseMousePosToGridPos(sectorDrawer.DrawLineStart);
            if (!sectorDrawer.DrawLineEnd.IsEmpty) sectorDrawer.DrawLineEnd = grid.ParseMousePosToGridPos(sectorDrawer.DrawLineEnd);
            UpdateGridSizeText();
        }

        void UpdateOriginPosText()
        {
            PointF pos = grid.GetOffsetOrigin();
            refData.lblOrigin.Text = string.Format("Origin: ({0}, {1})", pos.X, pos.Y);
        }

        void UpdateCursorPosText(Point mousePos)
        {
            float posX = Math.Sign(mousePos.X) * (Math.Abs(mousePos.X) - Grid.InitialOriginPos.X);
            float posY = Math.Sign(mousePos.Y) * (Math.Abs(mousePos.Y) - Grid.InitialOriginPos.Y);
            refData.lblCursor.Text = string.Format("Cursor: ({0}, {1})", posX, posY);
        }

        void UpdateGridSizeText() => refData.lblGridSize.Text = string.Format("GridSize: {0}", grid.GridSize);

        public void MoveOrigin(PointF delta)
        {
            grid.MoveOrigin(delta);
            UpdateOriginPosText();

            sectorDrawer.ShiftSectors(delta);
            if (delta.X != 0 || delta.Y != 0) refData.imgEditorDraw.Invalidate();
        }

        public void ExitCurrentMode()
        {
            if (sectorDrawer.IsDrawingLine)
            {
                sectorDrawer.ToggleLineDrawMode(false, cursor.MouseCurrentPos);
                return;
            }

            if (currentMode == EditorMode.None) return;
            currentMode = EditorMode.None;
            cursor.ToggleCursor(false);
            selectionManager.ResetData();
            refData.imgEditorDraw.Invalidate();
            sectorDrawer.DrawLineEnd = sectorDrawer.DrawLineStart = Point.Empty;
        }

        public void ChangeCurrentMode(EditorMode newMode)
        {
            ExitCurrentMode();
            currentMode = newMode;
            refData.imgEditorDraw.Invalidate();

            switch (currentMode)
            {
                case EditorMode.NodeMode: selectionManager.CurrentSelectionType = SelectionType.Node; break;
                case EditorMode.WallMode: selectionManager.CurrentSelectionType = SelectionType.Wall; break;
                case EditorMode.SectorMode: selectionManager.CurrentSelectionType = SelectionType.Sector; break;
            }
        }

        void MoveMapTimer_Tick(object sender, EventArgs e)
        {
            Point cursorPos = Cursor.Position;
            Point clientCursorPos = refData.imgEditorDraw.PointToClient(cursorPos);
            PointF cursorDelta = PointF.Subtract(cursorPos, new SizeF(lastMousePos));

            if (cursorDelta.X > 0 && clientCursorPos.X >= refData.imgEditorDraw.Right)
            {
                MoveOrigin(new Point(-grid.UnitSize, 0));
                cursorPos.X -= clientCursorPos.X - refData.imgEditorDraw.Right;
            }
            else if (cursorDelta.X < 0 && clientCursorPos.X <= 0)
            {
                MoveOrigin(new Point(grid.UnitSize, 0));
                cursorPos.X -= clientCursorPos.X;
            }

            if (cursorDelta.Y > 0 && clientCursorPos.Y >= refData.imgEditorDraw.Bottom)
            {
                MoveOrigin(new Point(0, -grid.UnitSize));
                cursorPos.Y -= clientCursorPos.Y - refData.imgEditorDraw.Bottom;
            }
            else if (cursorDelta.Y < 0 && clientCursorPos.Y <= 0)
            {
                MoveOrigin(new Point(0, grid.UnitSize));
                cursorPos.Y -= clientCursorPos.Y;
            }

            lastMousePos = Cursor.Position;
            Cursor.Position = cursorPos;
        }

        public void GetSectors(out List<Sector> sectors)
        {
            sectors = new List<Sector>();

            int sectSize = sectorDrawer.ActiveSectors.Count;
            for (int i = 0; i < sectSize; i++)
            {
                List<Wall> walls = new List<Wall>();

                int size = sectorDrawer.ActiveSectors[i].walls.Count;
                for (int j = 0; j < size; j++)
                {
                    Wall wall = sectorDrawer.ActiveSectors[i].walls[j];
                    wall.leftPoint = wall.leftPoint.Subtract(grid.GetCurrentOriginPos());
                    wall.rightPoint = wall.rightPoint.Subtract(grid.GetCurrentOriginPos());

                    walls.Add(wall);
                }

                sectors.Add(new Sector
                {
                    walls = walls,
                    ceillingHeight = sectorDrawer.ActiveSectors[i].ceillingHeight,
                    floorHeight = sectorDrawer.ActiveSectors[i].floorHeight,
                    ceillingColor = sectorDrawer.ActiveSectors[i].ceillingColor,
                    floorColor = sectorDrawer.ActiveSectors[i].floorColor,
                });
            }
        }

        public void LoadSectors(List<Sector> sectors)
        {
            int sectSize = sectors.Count();
            for (int i = 0; i < sectSize; i++)
            {
                int size = sectors[i].walls.Count;
                for (int j = 0; j < size; j++)
                {
                    Wall wall = sectors[i].walls[j];
                    wall.leftPoint = wall.leftPoint.Add(Grid.InitialOriginPos);
                    wall.rightPoint = wall.rightPoint.Add(Grid.InitialOriginPos);
                    wall.UpdateMiddleAndNormal();
                    sectors[i].walls[j] = wall;
                }
            }

            sectorDrawer.LoadSectors(ref sectors);
            refData.imgEditorDraw.Invalidate();
        }

        public void ResetData()
        {
            currentMode = EditorMode.None;
            grid.ResetData();
            selectionManager.ResetData();
            sectorDrawer.ResetData();

            UpdateOriginPosText();
            refData.imgEditorDraw.Invalidate();
        }

        public void OnResize()
        {
            grid.AdjustOrigin(refData.imgEditorDraw.Right, refData.imgEditorDraw.Bottom);
            refData.imgEditorDraw.Invalidate();
        }

        public void ChangedWallTopColor(Color newColor)
        {
            sectorDrawer.UpdateDrawnWallTopColor(newColor);
            selectionManager.UpdateWallColor(newColor, 0);
        }

        public void ChangedWallInnerColor(Color newColor)
        {
            sectorDrawer.UpdateDrawnWallInnerColor(newColor);
            selectionManager.UpdateWallColor(newColor, 1);
        }

        public void ChangedWallBottomColor(Color newColor)
        {
            sectorDrawer.UpdateDrawnWallBottomColor(newColor);
            selectionManager.UpdateWallColor(newColor, 2);
        }

        public void ChangedSectorFloorHeight(int newHeight)
        {
            sectorDrawer.UpdateDrawnSectorFloorHeight(newHeight);
            selectionManager.UpdateSectorFloorHeight(newHeight);
        }

        public void ChangedSectorCeillingHeight(int newHeight)
        {
            sectorDrawer.UpdateDrawnSectorCeillingHeight(newHeight);
            selectionManager.UpdateSectorCeillingHeight(newHeight);
        }

        public void ChangedSectorFloorColor(Color newColor)
        {
            sectorDrawer.UpdateDrawnSectorFloorColor(newColor);
            selectionManager.UpdateSectorFloorColor(newColor);
        }

        public void ChangedSectorCeillingColor(Color newColor)
        {
            sectorDrawer.UpdateDrawnSectorCeillingColor(newColor);
            selectionManager.UpdateSectorCeillingColor(newColor);
        }

        PointF GetMouseMovDelta(Point currentPos)
        {
            PointF delta = PointF.Subtract(currentPos, new SizeF(lastMousePos));
            float mag = delta.Magnitude();
            if (mag < 2f) return PointF.Empty;
            if (mag > .000001f) delta.Divide(mag);
            return delta;
        }

        public SelectionType GetCurrentSelectionType() => selectionManager.CurrentSelectionType;
        public SelectionData[] GetCurrentSelection() => selectionManager.GetCurrentSelection();

        void OnSelection(SelectionData data, bool hasMultipleSelections)
        {
            switch (selectionManager.CurrentSelectionType)
            {
                case SelectionType.Wall:
                    if (hasMultipleSelections)
                    {
                        refData.btnWallTopColor.ForeColor = refData.btnWallTopColor.BackColor = Color.Black;
                        refData.btnWallInColor.ForeColor = refData.btnWallInColor.BackColor = Color.Black;
                        refData.btnWallBtmColor.ForeColor = refData.btnWallBtmColor.BackColor = Color.Black;
                        refData.lblSelectionData.Enabled = false;
                    }

                    Wall selectedWall = sectorDrawer.ActiveSectors[data.sectorIndex].walls[data.wallIndex];

                    refData.btnWallTopColor.ForeColor = refData.btnWallTopColor.BackColor = selectedWall.colors[0];
                    refData.btnWallInColor.ForeColor = refData.btnWallInColor.BackColor = selectedWall.colors[1];
                    refData.btnWallBtmColor.ForeColor = refData.btnWallBtmColor.BackColor = selectedWall.colors[2];
                    refData.lblSelectionData.Enabled = true;

                    Point nodeA = selectedWall.leftPoint.Subtract(Grid.InitialOriginPos);
                    Point nodeB = selectedWall.rightPoint.Subtract(Grid.InitialOriginPos);
                    refData.lblSelectionData.Text = string.Format("Wall NodeA: {0} | NodeB: {1}", nodeA, nodeB);
                    break;

                case SelectionType.Sector:
                    if (refData.numbFloor.NumericUpDownControl == null || refData.numbCeilling.NumericUpDownControl == null) return;
                    if (hasMultipleSelections)
                    {
                        refData.btnWallTopColor.ForeColor = refData.btnWallTopColor.BackColor = Color.Black;
                        refData.btnWallInColor.ForeColor = refData.btnWallInColor.BackColor = Color.Black;
                        refData.btnWallBtmColor.ForeColor = refData.btnWallBtmColor.BackColor = Color.Black;
                        refData.btnFloorColor.ForeColor = refData.btnFloorColor.BackColor = Color.Black;
                        refData.btnCeillingColor.ForeColor = refData.btnCeillingColor.BackColor = Color.Black;
                        refData.numbFloor.NumericUpDownControl.Value = refData.numbCeilling.NumericUpDownControl.Value = 0;
                        refData.lblSelectionData.Enabled = false;
                    }

                    refData.btnWallTopColor.ForeColor = refData.btnWallTopColor.BackColor = CheckSectorWallsColor(sectorDrawer.ActiveSectors[data.sectorIndex].walls, 0);
                    refData.btnWallInColor.ForeColor = refData.btnWallInColor.BackColor = CheckSectorWallsColor(sectorDrawer.ActiveSectors[data.sectorIndex].walls, 1);
                    refData.btnWallBtmColor.ForeColor = refData.btnWallBtmColor.BackColor = CheckSectorWallsColor(sectorDrawer.ActiveSectors[data.sectorIndex].walls, 2);

                    refData.btnFloorColor.ForeColor = refData.btnFloorColor.BackColor = sectorDrawer.ActiveSectors[data.sectorIndex].floorColor;
                    refData.btnCeillingColor.ForeColor = refData.btnCeillingColor.BackColor = sectorDrawer.ActiveSectors[data.sectorIndex].ceillingColor;
                    refData.numbFloor.NumericUpDownControl.Value = sectorDrawer.ActiveSectors[data.sectorIndex].floorHeight;
                    refData.numbCeilling.NumericUpDownControl.Value = sectorDrawer.ActiveSectors[data.sectorIndex].ceillingHeight;
                    //refData.lblSelectionData.Enabled = true;
                    break;

                case SelectionType.Node:
                    if (hasMultipleSelections) refData.lblSelectionData.Enabled = false;
                    refData.lblSelectionData.Enabled = true;
                    Point node = sectorDrawer.ActiveSectors[data.sectorIndex].walls[data.wallIndex].leftPoint.Subtract(Grid.InitialOriginPos);
                    refData.lblSelectionData.Text = string.Format("Node: {0}", node);
                    break;
            }
        }

        Color CheckSectorWallsColor(List<Wall> walls, int colorIndx)
        {
            Color wallsColor = walls[0].colors[colorIndx];
            int size = walls.Count;
            for (int i = 1; i < size; i++)
            {
                if (walls[i].colors[colorIndx] == wallsColor) continue;
                return Color.Black;
            }

            return wallsColor;
        }

        void OnDeselect() => refData.lblSelectionData.Enabled = false;

        public void DeleteCurrentSelection()
        {
            selectionManager.DeleteCurrentSelection();
            refData.imgEditorDraw.Invalidate();
            refData.lblSelectionData.Enabled = false;
        }
    }
}
