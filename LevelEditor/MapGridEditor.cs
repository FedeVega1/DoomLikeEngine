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

    internal class MapGridEditor
    {
        const int DefaultUnitSize = 10, MinGridSize = 1, MaxGridSize = 8;
        static readonly Point InitialOriginPos = new Point(330, 210);
        static readonly Point MaxMapSizeX = new Point(-1500, 1500), MaxMapSizeY = new Point(-1500, 1500);

        bool isDraggingPanel, onLineMode;
        int gridSize = 3;
        Point originPosition, lastMousePos, mouseCurrentPos;
        System.Windows.Forms.Timer moveMapTimer;
        GridEditorData refData;

        int UnitSize => DefaultUnitSize * gridSize;

        public MapGridEditor(GridEditorData data)
        {
            refData = data;

            moveMapTimer = new System.Windows.Forms.Timer();
            originPosition = InitialOriginPos;
            lastMousePos = new Point(0, 0);

            refData.lblCursor.Enabled = false;
            UpdateOriginPosText();
            UpdateGridSizeText();
        }

        #region Events

        public void OnPaint(ref Graphics graph)
        {
            DrawEditorGrid(ref graph);
            DrawCursor(ref graph);
        }

        public void OnMouseDown(MouseButtons button, Point location)
        {
            if (onLineMode || button != MouseButtons.Left) return;
            isDraggingPanel = true;
            lastMousePos = location;
            refData.lblCursor.Enabled = false;
        }

        public void OnMouseMove(Point location)
        {
            if (onLineMode)
            {
                GetGridMousePos(location);
                UpdateCursorPosText(mouseCurrentPos);
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
            if (onLineMode)
            {
                Cursor.Hide();
                moveMapTimer.Interval = 50;
                moveMapTimer.Tick += MoveMapTimer_Tick;
                moveMapTimer.Start();
            }

            if (isDraggingPanel) return;
            refData.lblCursor.Enabled = true;
        }

        public void OnMouseLeave()
        {
            if (!onLineMode) 
                refData.lblCursor.Enabled = false;
        }

        #endregion

        public void UpdateGridSize(int sign)
        {
            sign = Math.Clamp(sign, -1, 1);
            int currentGridSize = gridSize;
            gridSize = Math.Clamp(gridSize + sign, MinGridSize, MaxGridSize);

            GetGridMousePos(mouseCurrentPos);
            UpdateGridSizeText();
            if (currentGridSize != gridSize) refData.imgEditorDraw.Invalidate();
        }

        void UpdateOriginPosText()
        {
            float posX = Math.Sign(originPosition.X) * (Math.Abs(originPosition.X) - InitialOriginPos.X);
            float posY = Math.Sign(originPosition.Y) * (Math.Abs(originPosition.Y) - InitialOriginPos.Y);
            refData.lblOrigin.Text = string.Format("Origin: ({0}, {1})", posX, posY);
        }

        void UpdateCursorPosText(Point mousePos)
        {
            float posX = Math.Sign(mousePos.X) * (Math.Abs(mousePos.X) - InitialOriginPos.X);
            float posY = Math.Sign(mousePos.Y) * (Math.Abs(mousePos.Y) - InitialOriginPos.Y);
            refData.lblCursor.Text = string.Format("Cursor: ({0}, {1})", posX, posY);
        }

        void UpdateGridSizeText() => refData.lblGridSize.Text = string.Format("GridSize: {0}", gridSize);

        public void MoveOrigin(PointF delta)
        {
            originPosition.X += (int) MathF.Round(delta.X);
            originPosition.Y += (int) MathF.Round(delta.Y);

            originPosition = originPosition.Clamp(MaxMapSizeX.X, MaxMapSizeX.Y, MaxMapSizeY.X, MaxMapSizeY.Y);

            UpdateOriginPosText();
            if (delta.X != 0 || delta.Y != 0) refData.imgEditorDraw.Invalidate();
        }

        void GetGridMousePos(Point mousePos)
        {
            Point diff = new Point(mousePos.X - originPosition.X, mousePos.Y - originPosition.Y);
            int x = originPosition.X + (UnitSize * (diff.X / UnitSize));
            int y = originPosition.Y + (UnitSize * (diff.Y / UnitSize));

            mouseCurrentPos = new Point(x, y);
        }

        void DrawEditorGrid(ref Graphics graph)
        {
            int panelWidth = (int) MathF.Round(graph.VisibleClipBounds.Right);
            int panelHeight = (int) MathF.Round(graph.VisibleClipBounds.Bottom);

            int sizeX = (Math.Abs(MaxMapSizeX.X) + MaxMapSizeX.Y) / UnitSize;
            int sizeY = (Math.Abs(MaxMapSizeY.X) + MaxMapSizeY.Y) / UnitSize;

            if (originPosition.X >= 0 && originPosition.X <= panelWidth)
                graph.DrawLine(Pens.Green, new Point(originPosition.X, 0), new Point(originPosition.X, panelHeight));

            if (originPosition.Y >= 0 && originPosition.Y <= panelHeight)
                graph.DrawLine(Pens.Red, new Point(0, originPosition.Y), new Point(panelWidth, originPosition.Y));

            for (int x = 0, i = 1, dir = 1; x < sizeX; x++)
            {
                int xPos = originPosition.X + (UnitSize * i++ * dir);

                if (xPos >= MaxMapSizeX.Y)
                {
                    dir *= -1;
                    i = 1;
                    xPos = originPosition.X + (UnitSize * i++ * dir);
                }

                if (xPos >= 0 && xPos <= panelWidth)
                    graph.DrawLine(Pens.Gray, new Point(xPos, 0), new Point(xPos, panelHeight));
            }

            for (int y = 0, i = 1, dir = 1; y < sizeY; y++)
            {
                int yPos = originPosition.Y + (UnitSize * i++ * dir);

                if (yPos >= MaxMapSizeY.Y)
                {
                    dir *= -1;
                    i = 1;
                    yPos = originPosition.Y + (UnitSize * i++ * dir);
                }

                if (yPos >= 0 && yPos <= panelHeight)
                    graph.DrawLine(Pens.DarkGray, new Point(0, yPos), new Point(panelWidth, yPos));
            }
        }

        void DrawCursor(ref Graphics graph)
        {
            if (!onLineMode) return;
            graph.DrawLine(Pens.Yellow, new Point(mouseCurrentPos.X, mouseCurrentPos.Y - 10), new Point(mouseCurrentPos.X, mouseCurrentPos.Y + 10));
            graph.DrawLine(Pens.Yellow, new Point(mouseCurrentPos.X - 10, mouseCurrentPos.Y), new Point(mouseCurrentPos.X + 10, mouseCurrentPos.Y));
        }

        public void ToggleLineMode(bool toggle)
        {
            if (!toggle)
            {
                onLineMode = false;
                Cursor.Show();
                moveMapTimer.Stop();
                refData.imgEditorDraw.Invalidate();
                return;
            }

            onLineMode = true;
        }

        void MoveMapTimer_Tick(object? sender, EventArgs e)
        {
            if (!onLineMode) return;
            Point cursorPos = Cursor.Position;
            Point clientCursorPos = refData.imgEditorDraw.PointToClient(cursorPos);
            PointF cursorDelta = PointF.Subtract(cursorPos, new SizeF(lastMousePos));

            if (cursorDelta.X > 0 && clientCursorPos.X >= refData.imgEditorDraw.Right)
            {
                MoveOrigin(new Point(-UnitSize, 0));
                cursorPos.X -= clientCursorPos.X - refData.imgEditorDraw.Right;
            }
            else if (cursorDelta.X < 0 && clientCursorPos.X <= 0)
            {
                MoveOrigin(new Point(UnitSize, 0));
                cursorPos.X -= clientCursorPos.X;
            }

            if (cursorDelta.Y > 0 && clientCursorPos.Y >= refData.imgEditorDraw.Bottom)
            {
                MoveOrigin(new Point(0, -UnitSize));
                cursorPos.Y -= clientCursorPos.Y - refData.imgEditorDraw.Bottom;
            }
            else if (cursorDelta.Y < 0 && clientCursorPos.Y <= 0)
            {
                MoveOrigin(new Point(0, UnitSize));
                cursorPos.Y -= clientCursorPos.Y;
            }

            lastMousePos = Cursor.Position;
            Cursor.Position = cursorPos;
        }
    }
}
