namespace LevelEditor
{
    readonly internal struct GridEditorData
    {
        public readonly Label lblCursor, lblOrigin, lblGridSize;
        public readonly PictureBox imgEditorDraw;

        public GridEditorData(ref Label cursor, ref Label origin, ref Label gridSize, ref PictureBox editorDraw)
        {
            lblCursor = cursor;
            lblOrigin = origin;
            lblGridSize = gridSize;
            imgEditorDraw = editorDraw;
        }
    }

    internal struct Wall
    {
        public Point leftPoint, rightPoint;
        public Color color;
        public PointF middle, normal;

        public Wall(Point left, Point right, Color c)
        {
            leftPoint = left;
            rightPoint = right;
            color = c;

            UpdateMiddleAndNormal();
        }

        public void UpdateMiddleAndNormal()
        {
            middle = leftPoint.Add(rightPoint.Subtract(leftPoint).Divide(2));
            normal = GetNormalFromPoints(leftPoint, rightPoint);
        }

        static PointF GetNormalFromPoints(Point a, Point b)
        {
            Point diff = b.Subtract(a);
            PointF dir = diff.Normalize();
            return new PointF(-dir.Y, dir.X);
        }
    }

    internal struct Sector
    {
        public List<Wall> walls;
        public int floorHeight, ceillingHeight;
        public Color floorColor, ceillingColor;

        public bool CheckWallsOrientation()
        {
            int size = walls.Count, sum = 0;
            for (int i = 0; i < size; i++)
            {
                int n = i + 1;
                if (n == size) n = 0;
                sum += (walls[i].leftPoint.Y + walls[n].leftPoint.Y) * (walls[n].leftPoint.X - walls[i].leftPoint.X);
            }

            return sum < 0;
        }

        public void FlipWalls()
        {
            walls.Reverse();
            int size = walls.Count;
            for (int i = 0; i < size; i++)
            {
                Wall wall = walls[i];
                Point p = wall.leftPoint;
                wall.leftPoint = wall.rightPoint;
                wall.rightPoint = p;
                wall.UpdateMiddleAndNormal();
                walls[i] = wall;
            }
        }
    }

    internal class MapCursor
    {
        System.Windows.Forms.Timer moveMapTimer;
        Action<object?, EventArgs> timerTick;

        public Point MouseCurrentPos { get; set; }

        public MapCursor(Action<object?, EventArgs> tick)
        {
            moveMapTimer = new System.Windows.Forms.Timer();
            timerTick = tick;
        }

        public void ToggleCursor(bool toggle)
        {
            if (toggle)
            {
                Cursor.Hide();
                moveMapTimer.Interval = 50;
                moveMapTimer.Tick += MoveMapTimer_Tick;
                moveMapTimer.Start();
                return;
            }

            Cursor.Show();
            moveMapTimer.Stop();
        }

        void MoveMapTimer_Tick(object? sender, EventArgs e) => timerTick?.Invoke(sender, e);

        public void DrawCursor(ref Graphics graph)
        {
            graph.DrawLine(Pens.Cyan, new Point(MouseCurrentPos.X, MouseCurrentPos.Y - 10), new Point(MouseCurrentPos.X, MouseCurrentPos.Y + 10));
            graph.DrawLine(Pens.Cyan, new Point(MouseCurrentPos.X - 10, MouseCurrentPos.Y), new Point(MouseCurrentPos.X + 10, MouseCurrentPos.Y));
        }

        public void UpdateCursorPos(Point pos) => MouseCurrentPos = pos;
    }

    internal class MapGridEditor
    {
        enum EditorMode { None, LineMode, NodeMode }

        bool isDraggingPanel, onLineMode;
        EditorMode currentMode;
        Point lastMousePos;
        GridEditorData refData;
        Grid grid;
        MapCursor cursor;
        Point drawLineStart, drawLineEnd;

        public bool IsDrawingLine => !drawLineStart.IsEmpty && !drawLineEnd.IsEmpty;

        readonly SolidBrush sectorBrush;
        readonly Pen wallLine;

        List<Sector> sectors;
        List<Wall> currentDrawnWalls;

        public MapGridEditor(GridEditorData data)
        {
            refData = data;
            sectors = new List<Sector>();
            currentDrawnWalls = new List<Wall>();

            grid = new Grid();
            cursor = new MapCursor(MoveMapTimer_Tick);

            sectorBrush = new SolidBrush(Color.FromArgb(0x77, 0xFF, 0xFF, 0xED));
            wallLine = new Pen(Color.Yellow, 2);

            refData.lblCursor.Enabled = false;
            UpdateOriginPosText();
            UpdateGridSizeText();
        }

        ~MapGridEditor()
        {
            sectorBrush.Dispose();
            wallLine.Dispose();
        }

        #region Events

        public void OnPaint(ref Graphics graph)
        {
            grid.DrawGrid(ref graph);
            DrawSector(ref graph);

            if (currentMode == EditorMode.LineMode)
            {
                DrawLine(ref graph);
                if (currentDrawnWalls.Count > 0) DrawWalls(ref graph, ref currentDrawnWalls, true);
                cursor.DrawCursor(ref graph);
            }
        }

        public void OnMouseDown(MouseButtons button, Point location)
        {
            if (button != MouseButtons.Left) return;

            if (currentMode == EditorMode.LineMode)
            {
                if (drawLineStart.IsEmpty && drawLineEnd.IsEmpty)
                {
                    ToggleLineDrawMode(true);
                    return;
                }

                currentDrawnWalls.Add(new Wall(drawLineStart, drawLineEnd, Color.White));
                drawLineEnd = drawLineStart = cursor.MouseCurrentPos;

                if (currentDrawnWalls[currentDrawnWalls.Count - 1].rightPoint == currentDrawnWalls[0].leftPoint)
                    MakeNewSector();
                return;
            }

            isDraggingPanel = true;
            lastMousePos = location;
            refData.lblCursor.Enabled = false;
        }

        public void OnMouseMove(Point location)
        {
            if (currentMode == EditorMode.LineMode)
            {
                cursor.MouseCurrentPos = grid.ParseMousePosToGridPos(location);
                if (!drawLineStart.IsEmpty) drawLineEnd = cursor.MouseCurrentPos;
                refData.imgEditorDraw.Invalidate();
            }
            else
                UpdateCursorPosText(location);

            if (!isDraggingPanel) return;

            PointF delta = PointF.Subtract(location, new SizeF(lastMousePos));
            float mag = delta.Magnitude();
            if (mag < 2f) return;
            if (mag > .000001f) delta.Divide(mag);

            MoveOrigin(delta);
            lastMousePos = location;
        }

        public void OnMouseUp(MouseButtons button)
        {
            if (button != MouseButtons.Left) return;
            isDraggingPanel = false;
            refData.lblCursor.Enabled = true;
        }

        public void OnMouseEnter()
        {
            if (currentMode == EditorMode.LineMode) cursor.ToggleCursor(true);

            if (isDraggingPanel) return;
            refData.lblCursor.Enabled = true;
        }

        public void OnMouseLeave()
        {
            if (currentMode != EditorMode.LineMode)
                refData.lblCursor.Enabled = false;
        }

        #endregion

        public void UpdateGridSize(int sign)
        {
            if (grid.UpdateGridSize(sign)) refData.imgEditorDraw.Invalidate();
            cursor.MouseCurrentPos = grid.ParseMousePosToGridPos(cursor.MouseCurrentPos);
            if (!drawLineStart.IsEmpty) drawLineStart = grid.ParseMousePosToGridPos(drawLineStart);
            if (!drawLineEnd.IsEmpty) drawLineEnd = grid.ParseMousePosToGridPos(drawLineEnd);
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

            int size = sectors.Count;
            for (int i = 0; i < size; i++)
            {
                int count = sectors[i].walls.Count;
                for (int j = 0; j < count; j++)
                {
                    Wall wall = sectors[i].walls[j];
                    wall.leftPoint.X += (int) MathF.Round(delta.X);
                    wall.leftPoint.Y += (int) MathF.Round(delta.Y);

                    wall.rightPoint.X += (int) MathF.Round(delta.X);
                    wall.rightPoint.Y += (int) MathF.Round(delta.Y);

                    wall.leftPoint = wall.leftPoint.Clamp(Grid.MaxMapSizeX.X, Grid.MaxMapSizeX.Y, Grid.MaxMapSizeY.X, Grid.MaxMapSizeY.Y);
                    wall.rightPoint = wall.rightPoint.Clamp(Grid.MaxMapSizeX.X, Grid.MaxMapSizeX.Y, Grid.MaxMapSizeY.X, Grid.MaxMapSizeY.Y);

                    wall.UpdateMiddleAndNormal();
                    sectors[i].walls[j] = wall;
                }
            }

            if (delta.X != 0 || delta.Y != 0) refData.imgEditorDraw.Invalidate();
        }

        public void ToggleLineMode(bool toggle)
        {
            if (!toggle)
            {
                currentMode = EditorMode.None;
                cursor.ToggleCursor(false);
                refData.imgEditorDraw.Invalidate();
                drawLineEnd = drawLineStart = Point.Empty;
                return;
            }

            currentMode = EditorMode.LineMode;
        }

        public void ToggleLineDrawMode(bool toggle)
        {
            if (!toggle)
            {
                drawLineEnd = drawLineStart = Point.Empty;
                currentDrawnWalls.Clear();
                return;
            }

            drawLineEnd = drawLineStart = cursor.MouseCurrentPos;
            if (currentDrawnWalls.Count > 0) currentDrawnWalls.Clear();
        }

        void DrawLine(ref Graphics graph)
        {
            if (drawLineStart.IsEmpty || drawLineEnd.IsEmpty) return;
            graph.DrawLine(wallLine, drawLineStart, drawLineEnd);

            Point diff = drawLineEnd.Subtract(drawLineStart);
            PointF middle = drawLineStart.Add(diff.Divide(2));
            graph.DrawLine(wallLine, middle, middle.Add(GetNormalFromPoints(diff).Multiply(5)));

            graph.FillRectangle(Brushes.AntiqueWhite, new Rectangle(drawLineStart.Subtract(3), new Size(6, 6)));
            graph.DrawRectangle(Pens.AntiqueWhite, new Rectangle(drawLineStart.Subtract(3), new Size(6, 6)));
        }

        void DrawWalls(ref Graphics graph, ref List<Wall> walls, bool showNodes)
        {
            int size = walls.Count;
            for (int i = 0; i < size; i++)
            {
                graph.DrawLine(wallLine, walls[i].leftPoint, walls[i].rightPoint);
                graph.DrawLine(wallLine, walls[i].middle, walls[i].middle.Add(walls[i].normal.Multiply(5)));

                if (showNodes)
                {
                    graph.FillRectangle(Brushes.AntiqueWhite, new Rectangle(walls[i].leftPoint.Subtract(3), new Size(6, 6)));
                    graph.DrawRectangle(Pens.AntiqueWhite, new Rectangle(walls[i].leftPoint.Subtract(3), new Size(6, 6)));
                }
            }
        }

        void DrawSector(ref Graphics graph)
        {
            int size = sectors.Count;
            for (int i = 0; i < size; i++)
            {
                List<Wall> walls = sectors[i].walls;
                DrawWalls(ref graph, ref walls, false);

                int wallsCount = walls.Count;
                Point[] wallPoints = new Point[wallsCount * 2];

                GetWallPoints(ref walls, ref wallPoints);
                graph.FillPolygon(sectorBrush, wallPoints, System.Drawing.Drawing2D.FillMode.Winding);
            }
        }

        void GetWallPoints(ref List<Wall> walls, ref Point[] wallPoints)
        {
            int count = wallPoints.Length;
            for (int j = 0, n = 0; n < count; j++, n++)
            {
                wallPoints[n++] = walls[j].leftPoint;
                wallPoints[n] = walls[j].rightPoint;
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

        PointF GetNormalFromPoints(Point diff)
        {
            PointF dir = diff.Normalize();
            return new PointF(-dir.Y, dir.X);
        }

        void MakeNewSector()
        {
            Sector newSector = new Sector { walls = new List<Wall>(currentDrawnWalls) };
            if (!newSector.CheckWallsOrientation()) newSector.FlipWalls();

            sectors.Add(newSector);

            drawLineEnd = drawLineStart = Point.Empty;
            currentDrawnWalls.Clear();

            refData.imgEditorDraw.Invalidate();
        }

        public void GetSectors(out List<Sector> sectors) => sectors = new List<Sector>(this.sectors);

        public void LoadSectors(List<Sector> sectors)
        {
            this.sectors = sectors;
            refData.imgEditorDraw.Invalidate();
        }

        public void ResetData()
        {
            sectors.Clear();
            currentMode = EditorMode.None;
            grid.ResetData();

            UpdateOriginPosText();
            refData.imgEditorDraw.Invalidate();
        }
    }
}
