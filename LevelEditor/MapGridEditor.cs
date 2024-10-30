namespace LevelEditor
{
    readonly internal struct GridEditorData
    {
        public readonly ToolStripStatusLabel lblCursor, lblOrigin, lblGridSize;
        public readonly PictureBox imgEditorDraw;

        public GridEditorData(ref ToolStripStatusLabel cursor, ref ToolStripStatusLabel origin, ref ToolStripStatusLabel gridSize, ref PictureBox editorDraw)
        {
            lblCursor = cursor;
            lblOrigin = origin;
            lblGridSize = gridSize;
            imgEditorDraw = editorDraw;
        }
    }

    internal class MapCursor
    {
        System.Windows.Forms.Timer moveMapTimer;
        Action<object?, EventArgs> timerTick;

        public bool CursorIsActive { get; private set; }
        public Point MouseCurrentPos { get; set; }

        public MapCursor(Action<object?, EventArgs> tick)
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

        void MoveMapTimer_Tick(object? sender, EventArgs e) => timerTick?.Invoke(sender, e);

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
            sectorDrawer = new SectorDrawer();
            selectionManager = new SelectionManager(ref sectorDrawer, ref grid);

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

                    break;

                case EditorMode.SectorMode:

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
                    sectorDrawer.OnMouseMove(cursor.MouseCurrentPos);
                    refData.imgEditorDraw.Invalidate();
                    return;

                case EditorMode.NodeMode:
                case EditorMode.WallMode:
                case EditorMode.SectorMode:
                    selectionManager.OnMouseMove(grid.ParseMousePosToGridPos(location));
                    refData.imgEditorDraw.Invalidate();
                    lastMousePos = location;
                    if (selectionManager.HasSelection) return;
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
            if (currentMode == EditorMode.LineMode) cursor.ToggleCursor(true);

            if (isDraggingPanel) return;
            refData.lblCursor.Visible = true;
        }

        public void OnMouseLeave()
        {
            if (currentMode != EditorMode.LineMode)
                refData.lblCursor.Visible = false;
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

        void MoveMapTimer_Tick(object? sender, EventArgs e)
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
            sectors = new List<Sector>(sectorDrawer.ActiveSectors);

            int sectSize = sectors.Count;
            for (int i = 0; i < sectSize; i++)
            {
                int size = sectors[i].walls.Count;
                for (int j = 0; j < size; j++)
                {
                    Wall wall = sectors[i].walls[j];
                    wall.leftPoint = wall.leftPoint.Subtract(Grid.InitialOriginPos);
                    wall.rightPoint = wall.rightPoint.Subtract(Grid.InitialOriginPos);
                    sectors[i].walls[j] = wall;
                }
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
                    sectors[i].walls[j] = wall;
                }
            }

            sectorDrawer.LoadSectors(ref sectors);
            refData.imgEditorDraw.Invalidate();
        }

        public void ResetData()
        {
            sectorDrawer.ActiveSectors.Clear();
            currentMode = EditorMode.None;
            grid.ResetData();

            UpdateOriginPosText();
            refData.imgEditorDraw.Invalidate();
        }

        public void OnResize()
        {
            grid.AdjustOrigin(refData.imgEditorDraw.Right, refData.imgEditorDraw.Bottom);
            refData.imgEditorDraw.Invalidate();
        }

        public void ChangedWallColor(Color newColor)
        {
            sectorDrawer.UpdateDrawnWallColor(newColor);
            selectionManager.UpdateWallColor(newColor);
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
    }
}
