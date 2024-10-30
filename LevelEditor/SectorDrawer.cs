using System.Reflection.Metadata.Ecma335;

namespace LevelEditor
{
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
        public Color floorColor, ceillingColor;
        public int floorHeight, ceillingHeight;

        public Sector()
        {
            walls = new List<Wall>();
            floorHeight = 0;
            ceillingHeight = 10;
            ceillingColor = floorColor = Color.Black;
        }

        public Sector(Sector sector)
        {
            walls = new List<Wall>(sector.walls);
            floorHeight = sector.floorHeight;
            ceillingHeight = sector.ceillingHeight;
            floorColor = sector.floorColor;
            ceillingColor = sector.ceillingColor;
        }

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

        public void ResetDrawSector()
        {
            walls.Clear();
            floorHeight = 0;
            ceillingHeight = 10;
            ceillingColor = floorColor = Color.Black;
        }
    }

    internal struct SectorDrawingData
    {
        public bool showWallNodes;
        public int[] selectedWalls;
    }

    internal class SectorDrawer
    {
        List<Sector> sectors;
        Sector currentDrawnSector;
        Color currentDrawnWallsColor;

        public Point DrawLineStart { get; set; }
        public Point DrawLineEnd { get; set; }

        public bool IsDrawingLine => !DrawLineStart.IsEmpty && !DrawLineEnd.IsEmpty;
        public bool IsNotDrawingLine => DrawLineStart.IsEmpty && DrawLineEnd.IsEmpty;

        public List<Sector> ActiveSectors => sectors;
        public Sector CurrentDrawnSector => currentDrawnSector;

        readonly SolidBrush sectorBrush;//, displaytextBrush;
        readonly Pen wallLine;
        //readonly Font displayFont;

        public SectorDrawer()
        {
            sectors = new List<Sector>();
            currentDrawnSector = new Sector();

            sectorBrush = new SolidBrush(Color.FromArgb(0x77, 0xFF, 0xFF, 0xED));
            wallLine = new Pen(Color.Yellow, 2);
            //displayFont = new Font("Roboto", 24);
            //displaytextBrush = new SolidBrush(Color.FromArgb(0xBB, 0x46, 0x9E, 0x94));
        }

        ~SectorDrawer()
        {
            sectorBrush.Dispose();
            wallLine.Dispose();
            //displayFont.Dispose();
        }

        public void OnLineModeDraw(ref Graphics graph)
        {
            DrawLine(ref graph);
            if (currentDrawnSector.walls.Count > 0) DrawWalls(ref graph, ref currentDrawnSector.walls, true);
        }

        public bool OnMouseDown(Point cursorPos)
        {
            currentDrawnSector.walls.Add(new Wall(DrawLineStart, DrawLineEnd, currentDrawnWallsColor));
            DrawLineEnd = DrawLineStart = cursorPos;

            if (currentDrawnSector.walls[currentDrawnSector.walls.Count - 1].rightPoint == currentDrawnSector.walls[0].leftPoint)
            {
                MakeNewSector();
                return true;
            }

            return false;
        }

        public void OnMouseMove(Point cursorPos)
        {
            if (!DrawLineStart.IsEmpty) 
                DrawLineEnd = cursorPos;
        }

        public void ShiftSectors(PointF delta)
        {
            LoopSectorWalls((int i, int j, Wall wall) =>
            {
                wall.leftPoint.X += (int) MathF.Round(delta.X);
                wall.leftPoint.Y += (int) MathF.Round(delta.Y);

                wall.rightPoint.X += (int) MathF.Round(delta.X);
                wall.rightPoint.Y += (int) MathF.Round(delta.Y);

                wall.leftPoint = wall.leftPoint.Clamp(-Grid.MaxMapSize.X, Grid.MaxMapSize.X, -Grid.MaxMapSize.Y, Grid.MaxMapSize.Y);
                wall.rightPoint = wall.rightPoint.Clamp(-Grid.MaxMapSize.X, Grid.MaxMapSize.X, -Grid.MaxMapSize.Y, Grid.MaxMapSize.Y);

                wall.UpdateMiddleAndNormal();
                sectors[i].walls[j] = wall;
                return false;
            });
        }

        void DrawLine(ref Graphics graph)
        {
            if (DrawLineStart.IsEmpty || DrawLineEnd.IsEmpty) return;
            graph.DrawLine(wallLine, DrawLineStart, DrawLineEnd);

            Point diff = DrawLineEnd.Subtract(DrawLineStart);
            PointF middle = DrawLineStart.Add(diff.Divide(2));
            graph.DrawLine(wallLine, middle, middle.Add(GetNormalFromPoints(diff).Multiply(5)));

            graph.FillRectangle(Brushes.AntiqueWhite, new Rectangle(DrawLineStart.Subtract(3), new Size(6, 6)));
            graph.DrawRectangle(Pens.AntiqueWhite, new Rectangle(DrawLineStart.Subtract(3), new Size(6, 6)));
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

        public void DrawSector(ref Graphics graph, SectorDrawingData data = default)
        {
            int size = sectors.Count;
            for (int i = 0; i < size; i++)
            {
                List<Wall> walls = sectors[i].walls;
                DrawWalls(ref graph, ref walls, data.showWallNodes);

                int wallsCount = walls.Count;
                Point[] wallPoints = new Point[wallsCount * 2];

                GetWallPoints(ref walls, ref wallPoints);
                graph.FillPolygon(sectorBrush, wallPoints, System.Drawing.Drawing2D.FillMode.Winding);
            }
        }

        void MakeNewSector()
        {
            Sector newSector = new Sector(currentDrawnSector);
            if (!newSector.CheckWallsOrientation()) newSector.FlipWalls();

            sectors.Add(newSector);

            DrawLineEnd = DrawLineStart = Point.Empty;
            currentDrawnSector.ResetDrawSector();

        }

        public void ToggleLineDrawMode(bool toggle, Point CursorPos)
        {
            if (!toggle)
            {
                DrawLineEnd = DrawLineStart = Point.Empty;
                currentDrawnSector.ResetDrawSector();
                return;
            }

            DrawLineEnd = DrawLineStart = CursorPos;
            if (currentDrawnSector.walls.Count > 0) currentDrawnSector.ResetDrawSector();
        }

        static void GetWallPoints(ref List<Wall> walls, ref Point[] wallPoints)
        {
            int count = wallPoints.Length;
            for (int j = 0, n = 0; n < count; j++, n++)
            {
                wallPoints[n++] = walls[j].leftPoint;
                wallPoints[n] = walls[j].rightPoint;
            }
        }

        static PointF GetNormalFromPoints(Point diff)
        {
            PointF dir = diff.Normalize();
            return new PointF(-dir.Y, dir.X);
        }

        public void LoadSectors(ref List<Sector> sectors) => this.sectors = sectors;

        public void UpdateDrawnSectorFloorHeight(int newHeight) => currentDrawnSector.floorHeight = newHeight;
        public void UpdateDrawnSectorCeillingHeight(int newHeight) => currentDrawnSector.ceillingHeight = newHeight;
        public void UpdateDrawnWallColor(Color color) => currentDrawnWallsColor = color;
        public void UpdateDrawnSectorFloorColor(Color color) => currentDrawnSector.floorColor = color;
        public void UpdateDrawnSectorCeillingColor(Color color) => currentDrawnSector.ceillingColor = color;

        public bool CheckForNodes(Point mousePoint, float dist, out (int, int) indx)
        {
            (int, int) foundIndx = (-1, -1);
            bool found = false;

            LoopSectorWalls((int i, int j, Wall wall) =>
            {
                float lftdist = wall.leftPoint.Distance(mousePoint);

                if (lftdist <= dist)
                {
                    foundIndx.Item1 = i;
                    foundIndx.Item2 = j;
                    found = true;
                    return true;
                }

                return false;
            }, true);

            indx = foundIndx;
            return found;
        }

        public void LoopSectorWalls(Func<int, int, Wall, bool> callback, bool breakOrReturn = false)
        {
            if (callback == null) return;

            int sectorSize = sectors.Count;
            for (int i = 0; i < sectorSize; i++)
            {
                int wallsSize = sectors[i].walls.Count;
                for (int j = 0; j < wallsSize; j++)
                {
                    if (callback(i, j, sectors[i].walls[j]))
                    {
                        if (breakOrReturn) break;
                        else return;
                    }
                }
            }
        }
    }
}
