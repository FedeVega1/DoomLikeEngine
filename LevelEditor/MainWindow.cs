namespace LevelEditor
{
    public partial class MainWindow : Form
    {
        const int DefaultUnitSize = 10, MinGridSize = 1, MaxGridSize = 8;
        static readonly Point InitialOriginPos = new Point(330, 210);
        static readonly Point MaxMapSizeX = new Point(-1500, 1500), MaxMapSizeY = new Point(-1500, 1500);

        int gridSize = 3;
        Point originPosition;
        bool isDraggingPanel;
        Point lastMousePos;

        int UnitSize => DefaultUnitSize * gridSize;

        public MainWindow()
        {
            InitializeComponent();
            COLoggerImport.InitLogSys(true, false);

            originPosition = InitialOriginPos;
            lastMousePos = new Point(0, 0);

            LblCursor.Enabled = false;
            UpdateOriginPosText();
            UpdateGridSizeText();
        }

        void ImgEditorDraw_Paint(object sender, PaintEventArgs e)
        {
            Graphics graph = e.Graphics;

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
                    graph.DrawLine(Pens.White, new Point(xPos, 0), new Point(xPos, panelHeight));
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
                    graph.DrawLine(Pens.Gray, new Point(0, yPos), new Point(panelWidth, yPos));
            }
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            int currentGridSize;
            switch (keyData)
            {
                case Keys.Add:
                    currentGridSize = gridSize;
                    gridSize = Math.Clamp(++gridSize, MinGridSize, MaxGridSize);

                    UpdateGridSizeText();
                    if (currentGridSize != gridSize) ImgEditorDraw.Invalidate();
                    return true;

                case Keys.Subtract:
                    currentGridSize = gridSize;
                    gridSize = Math.Clamp(--gridSize, MinGridSize, MaxGridSize);

                    UpdateGridSizeText();
                    if (currentGridSize != gridSize) ImgEditorDraw.Invalidate();
                    return true;

                case Keys.Up: MoveOrigin(new PointF(0, -1)); return true;
                case Keys.Down: MoveOrigin(new PointF(0, 1)); return true;
                case Keys.Left: MoveOrigin(new PointF(-1, 0)); return true;
                case Keys.Right: MoveOrigin(new PointF(1, 0)); return true;
            }

            return base.ProcessCmdKey(ref msg, keyData);
        }

        void ImgEditorDraw_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left) return;
            isDraggingPanel = true;
            lastMousePos = e.Location;
            LblCursor.Enabled = false;
        }

        void ImgEditorDraw_MouseMove(object sender, MouseEventArgs e)
        {
            UpdateCursorPosText(e.Location);
            if (!isDraggingPanel) return;

            PointF delta = PointF.Subtract(e.Location, new SizeF(lastMousePos));
            float mag = delta.Magnitude();
            if (mag < 2f) return;
            if (mag > .000001f) delta.Divide(mag);

            MoveOrigin(delta);
            lastMousePos = e.Location;
        }

        void ImgEditorDraw_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left) return;
            isDraggingPanel = false;
            LblCursor.Enabled = true;
        }

        void UpdateOriginPosText()
        {
            float posX = Math.Sign(originPosition.X) * (Math.Abs(originPosition.X) - InitialOriginPos.X);
            float posY = Math.Sign(originPosition.Y) * (Math.Abs(originPosition.Y) - InitialOriginPos.Y);
            LblOrigin.Text = string.Format("Origin: ({0}, {1})", posX, posY);
        }

        void UpdateCursorPosText(Point mousePos) => LblCursor.Text = string.Format("Cursor: ({0}, {1})", mousePos.X, mousePos.Y);

        void UpdateGridSizeText() => LblGridSize.Text = string.Format("GridSize: {0}", gridSize);

        void MoveOrigin(PointF delta)
        {
            originPosition.X += (int) (1 * MathF.Round(delta.X));
            originPosition.Y += (int) (1 * MathF.Round(delta.Y));

            originPosition = originPosition.Clamp(MaxMapSizeX.X, MaxMapSizeX.Y, MaxMapSizeY.X, MaxMapSizeY.Y);

            UpdateOriginPosText();
            if (delta.X != 0 || delta.Y != 0) ImgEditorDraw.Invalidate();
        }

        private void ImgEditorDraw_MouseEnter(object sender, EventArgs e)
        {
            if (isDraggingPanel) return;
            LblCursor.Enabled = true;
        }

        private void ImgEditorDraw_MouseLeave(object sender, EventArgs e)
        {
            LblCursor.Enabled = false;
        }
    }
}
