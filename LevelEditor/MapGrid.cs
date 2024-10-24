namespace LevelEditor
{
    internal class Grid
    {
        const int DefaultUnitSize = 10, MinGridSize = 1, MaxGridSize = 8;
        public static readonly Point InitialOriginPos = new Point(330, 210);
        public static readonly Point MaxMapSizeX = new Point(-1500, 1500), MaxMapSizeY = new Point(-1500, 1500);

        int gridSize = 3;
        Point originPosition;

        readonly Pen originVertical, originHorizontal;
        readonly Pen verticalOdd, verticalEven;
        readonly Pen horizontalOdd, horizontalEven;

        public int GridSize => gridSize;
        public int UnitSize => DefaultUnitSize * gridSize;

        public Grid()
        {
            originPosition = InitialOriginPos;

            float[] dash = { .5f, .5f };
            originVertical = new Pen(Color.LimeGreen, 2);
            originVertical.DashPattern = dash;

            originHorizontal = new Pen(Color.Red, 2);
            originHorizontal.DashPattern = dash;

            verticalOdd = new Pen(Color.LightGray, 2);
            verticalOdd.DashPattern = dash;

            verticalEven = new Pen(Color.LightSlateGray, 2);
            verticalEven.DashPattern = dash;

            horizontalOdd = new Pen(Color.DarkGray, 2);
            horizontalOdd.DashPattern = dash;

            horizontalEven = new Pen(Color.DarkSlateGray, 2);
            horizontalEven.DashPattern = dash;
        }

        ~Grid()
        {
            originVertical.Dispose();
            originHorizontal.Dispose();
            verticalOdd.Dispose();
            verticalEven.Dispose();
            horizontalOdd.Dispose();
            horizontalEven.Dispose();
        }

        public bool UpdateGridSize(int sign)
        {
            sign = Math.Clamp(sign, -1, 1);
            int currentGridSize = gridSize;
            gridSize = Math.Clamp(gridSize + sign, MinGridSize, MaxGridSize);
            return currentGridSize != gridSize;
        }

        public void MoveOrigin(PointF delta)
        {
            originPosition.X += (int) MathF.Round(delta.X);
            originPosition.Y += (int) MathF.Round(delta.Y);

            originPosition = originPosition.Clamp(MaxMapSizeX.X, MaxMapSizeX.Y, MaxMapSizeY.X, MaxMapSizeY.Y);
        }

        public Point ParseMousePosToGridPos(Point mousePos)
        {
            Point diff = new Point(mousePos.X - originPosition.X, mousePos.Y - originPosition.Y);
            int x = originPosition.X + (UnitSize * (diff.X / UnitSize));
            int y = originPosition.Y + (UnitSize * (diff.Y / UnitSize));
            return new Point(x, y);
        }

        public PointF GetOffsetOrigin()
        {
            float posX = Math.Sign(originPosition.X) * (Math.Abs(originPosition.X) - InitialOriginPos.X);
            float posY = Math.Sign(originPosition.Y) * (Math.Abs(originPosition.Y) - InitialOriginPos.Y);
            return new PointF(posX, posY);
        }

        public void ResetData()
        {
            originPosition = InitialOriginPos;
        }

        public void DrawGrid(ref Graphics graph)
        {
            int panelWidth = (int) MathF.Round(graph.VisibleClipBounds.Right);
            int panelHeight = (int) MathF.Round(graph.VisibleClipBounds.Bottom);

            int sizeX = (Math.Abs(MaxMapSizeX.X) + MaxMapSizeX.Y) / UnitSize;
            int sizeY = (Math.Abs(MaxMapSizeY.X) + MaxMapSizeY.Y) / UnitSize;

            if (originPosition.X >= 0 && originPosition.X <= panelWidth)
                graph.DrawLine(originVertical, new Point(originPosition.X, 0), new Point(originPosition.X, panelHeight));

            if (originPosition.Y >= 0 && originPosition.Y <= panelHeight)
                graph.DrawLine(originHorizontal, new Point(0, originPosition.Y), new Point(panelWidth, originPosition.Y));

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
                    graph.DrawLine(i % 2 == 0 ? verticalEven : verticalOdd, new Point(xPos, 0), new Point(xPos, panelHeight));
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
                    graph.DrawLine(i % 2 == 0 ? horizontalEven : horizontalOdd, new Point(0, yPos), new Point(panelWidth, yPos));
            }
        }
    }
}
