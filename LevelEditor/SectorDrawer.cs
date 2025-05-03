namespace LevelEditor
{
    internal class Wall
    {
        public struct WallColors
        {
            public Color topColor, inColor, btmColor;

            public Color this[int indx]
            {
                get => indx switch
                {
                    0 => topColor,
                    1 => inColor,
                    _ => btmColor,
                };

                set
                {
                    switch (indx)
                    {
                        case 0: topColor = value; break;
                        case 1: inColor = value; break;
                        default: btmColor = value; break;
                    }
                }
            }

            public Color[] ToArray() { return new Color[3] { topColor, inColor, btmColor }; }
        }

        static ulong WallCounter = 0;

        public Point leftPoint, rightPoint;
        public WallColors colors;
        public PointF middle, normal;
        public bool isPortal, isConnection;
        public int portalTargetSector, portalTargetWall;
        public readonly ulong wallID;
        
        public Sector Sector { get; private set; }

        public Wall(Sector s)
        {
            leftPoint = rightPoint = Point.Empty;
            colors = new WallColors();
            isPortal = false;
            portalTargetSector = -1;
            portalTargetWall = -1;
            wallID = WallCounter++;
            Sector = s;
        }

        public Wall(Point left, Point right, Color[] c, Sector s)
        {
            leftPoint = left;
            rightPoint = right;

            colors[0] = c[0];
            colors[1] = c[1];
            colors[2] = c[2];

            isPortal = false;
            portalTargetSector = -1;
            portalTargetWall = -1;

            Sector = s;

            UpdateMiddleAndNormal();
            wallID = WallCounter++;
        }

        public Wall(Point newLeft, Point newRight, Wall wallCopy)
        {
            leftPoint = newLeft;
            rightPoint = newRight;

            colors = wallCopy.colors;

            isPortal = wallCopy.isPortal;
            portalTargetSector = wallCopy.portalTargetSector;
            portalTargetWall = wallCopy.portalTargetWall;

            Sector = wallCopy.Sector;

            UpdateMiddleAndNormal();
            wallID = wallCopy.wallID;
        }

        //public Wall(Wall wallCopy)
        //{
        //    leftPoint = wallCopy.leftPoint;
        //    rightPoint = wallCopy.rightPoint;

        //    colors = wallCopy.colors;

        //    isPortal = wallCopy.isPortal;
        //    portalTargetSector = wallCopy.portalTargetSector;
        //    portalTargetWall = wallCopy.portalTargetWall;

        //    UpdateMiddleAndNormal();
        //    wallID = wallCopy.wallID;
        //}

        public void UpdateMiddleAndNormal()
        {
            middle = leftPoint.Add(rightPoint.Subtract(leftPoint).Divide(2));
            normal = GetNormalFromPoints(leftPoint, rightPoint);
        }

        public Wall MakePortal(int sector, int wall, bool isConnection = false)
        {
            portalTargetSector = sector;
            portalTargetWall = wall;
            isPortal = true;
            this.isConnection = isConnection;
            return this;
        }

        static PointF GetNormalFromPoints(Point a, Point b)
        {
            Point diff = b.Subtract(a);
            PointF dir = diff.Normalize();
            return new PointF(dir.Y, -dir.X);
        }

        public Color[] GetColors() { return colors.ToArray(); }
    }

    internal class Sector
    {
        static uint SectorCounter = 0;
        public uint SectorID { get; private set; }

        public List<Wall> walls;
        public Color floorColor, ceillingColor;
        public int floorHeight, ceillingHeight;
        public Point centroid, min, max;

        public Sector()
        {
            walls = new List<Wall>();
            floorHeight = 0;
            ceillingHeight = 10;
            ceillingColor = floorColor = Color.Black;

            UpdateSectorValues();
        }

        public Sector(List<Wall> wallList, int fH, int cH, Color fC, Color cC, uint sectorID)
        {
            walls = wallList;
            floorHeight = fH;
            ceillingHeight = cH;
            ceillingColor = cC;
            floorColor = fC;
            SectorID = sectorID;

            UpdateSectorValues();
        }

        public Sector(uint newID)
        {
            SectorID = newID;
            if (SectorCounter < SectorID) SectorCounter = SectorID;

            walls = new List<Wall>();
            floorHeight = 0;
            ceillingHeight = 10;
            ceillingColor = floorColor = Color.Black;

            UpdateSectorValues();
        }

        public Sector(Sector sector)
        {
            SectorID = SectorCounter++;

            walls = new List<Wall>(sector.walls);
            floorHeight = sector.floorHeight;
            ceillingHeight = sector.ceillingHeight;
            floorColor = sector.floorColor;
            ceillingColor = sector.ceillingColor;

            UpdateSectorValues();
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

            return sum >= 0;
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
            //floorHeight = 0;
            //ceillingHeight = 10;
            //ceillingColor = floorColor = Color.Black;
        }

        public void UpdateSectorValues()
        {
            centroid = CalculateCentroid();
            (Point, Point) minMax = GetMinMaxPoints();
            min = minMax.Item1;
            max = minMax.Item2;
        }

        public Point CalculateCentroid()
        {
            if (walls.Count == 0) return Point.Empty;

            Point centroid = Point.Empty;
            int sumArea = 0, area = 0;

            int size = walls.Count;
            for (int i = 0; i < size; i++)
            {
                area = walls[i].leftPoint.X * walls[i].rightPoint.Y - walls[i].rightPoint.X * walls[i].leftPoint.Y;
                sumArea += area;
                centroid.X += (walls[i].leftPoint.X + walls[i].rightPoint.X) * area;
                centroid.Y += (walls[i].leftPoint.Y + walls[i].rightPoint.Y) * area;
            }

            centroid.Divide(sumArea * 3);
            return centroid;
        }

        public (Point, Point) GetMinMaxPoints()
        {
            Point min = new Point(int.MaxValue, int.MaxValue), max = new Point(int.MinValue, int.MinValue);
            int wallsSize = walls.Count;
            for (int i = 0; i < wallsSize; i++)
            {
                if (walls[i].leftPoint.X < min.X) min.X = walls[i].leftPoint.X;
                else if (walls[i].leftPoint.X > max.X) max.X = walls[i].leftPoint.X;

                if (walls[i].rightPoint.X < min.X) min.X = walls[i].rightPoint.X;
                else if (walls[i].rightPoint.X > max.X) max.X = walls[i].rightPoint.X;

                if (walls[i].leftPoint.Y < min.Y) min.Y = walls[i].leftPoint.Y;
                else if (walls[i].leftPoint.Y > max.Y) max.Y = walls[i].leftPoint.Y;

                if (walls[i].rightPoint.Y < min.Y) min.Y = walls[i].rightPoint.Y;
                else if (walls[i].rightPoint.Y > max.Y) max.Y = walls[i].rightPoint.Y;
            }

            return (min, max);
        }

        public bool PointInsideSector(Point point)
        {
            return point.X < min.X || point.X > max.X || point.Y < min.Y || point.Y > max.Y;
        }
    }

    internal struct SectorDrawingData
    {
        public bool showWallNodes;
        public int[] selectedWalls;
    }

    internal class SectorDrawer
    {
        bool mouseOnGrid;
        Sector currentDrawnSector;
        Color[] currentDrawnWallsColor;
        Point currentCursorPos;
        List<Sector> sectors;
        MapGridEditor editor;

        public Point DrawLineStart { get; set; }
        public Point DrawLineEnd { get; set; }

        public bool IsDrawingLine => !DrawLineStart.IsEmpty && !DrawLineEnd.IsEmpty;
        public bool IsNotDrawingLine => DrawLineStart.IsEmpty && DrawLineEnd.IsEmpty;

        public List<Sector> ActiveSectors => sectors;
        public Sector CurrentDrawnSector => currentDrawnSector;

        readonly SolidBrush sectorBrush, hoverSectorBrush, selectedSectorBrush;
        readonly SolidBrush nodeBrush, hoverNodeBrush, selectedNodeBrush;
        readonly Pen wallLine, hoverWallLine, selectedWallLine;
        readonly Pen portalConnectionLine, hoverPortalConnectionLine, selectedPortalConnectionLine;
        readonly Pen portalLine, hoverPortalLine, selectedPortalLine;
        readonly Pen nodeLine, hoverNodeLine, selectedNodeLine;

        public SectorDrawer(MapGridEditor gridEditor)
        {
            editor = gridEditor;

            currentDrawnWallsColor = new Color[3];
            sectors = new List<Sector>();
            currentDrawnSector = new Sector();

            sectorBrush = new SolidBrush(Color.FromArgb(0x77, 0xFF, 0xFF, 0xED));
            hoverSectorBrush = new SolidBrush(Color.FromArgb(0x77, 0xFC, 0xF2, 0xD9));
            selectedSectorBrush = new SolidBrush(Color.FromArgb(0x77, 0xFC, 0xDD, 0xC0));

            wallLine = new Pen(Color.Yellow, 2);
            hoverWallLine = new Pen(Color.Orange, 3);
            selectedWallLine = new Pen(Color.OrangeRed, 2);

            portalConnectionLine = new Pen(Color.LightGray, 1.5f);
            hoverPortalConnectionLine = new Pen(Color.FromArgb(0xFF, 0xFF, 0xC2, 0x66), 2);
            selectedPortalConnectionLine = new Pen(Color.FromArgb(0xFF, 0xFF, 0xA3, 0x66), 1.5f);

            portalLine = new Pen(Color.FromArgb(0xFF, 0xFF, 0x66, 0xCC), 2);
            hoverPortalLine = new Pen(Color.FromArgb(0xFF, 0xFF, 0x33, 0xBB), 3);
            selectedPortalLine = new Pen(Color.FromArgb(0xFF, 0xFF, 0, 0xAA), 2);

            nodeBrush = new SolidBrush(Color.AntiqueWhite);
            hoverNodeBrush = new SolidBrush(Color.LightBlue);
            selectedNodeBrush = new SolidBrush(Color.Blue);

            nodeLine = new Pen(Color.AntiqueWhite, 1);
            hoverNodeLine = new Pen(Color.LightBlue, 2);
            selectedNodeLine = new Pen(Color.Blue, 1);
        }

        ~SectorDrawer()
        {
            sectorBrush.Dispose();
            hoverSectorBrush.Dispose();
            selectedSectorBrush.Dispose();
            wallLine.Dispose();
            hoverWallLine.Dispose();
            selectedWallLine.Dispose();
            portalConnectionLine.Dispose();
            hoverPortalConnectionLine.Dispose();
            selectedPortalConnectionLine.Dispose();
            portalLine.Dispose();
            hoverPortalLine.Dispose();
            selectedPortalLine.Dispose();
            nodeBrush.Dispose();
            hoverNodeBrush.Dispose();
            selectedNodeBrush.Dispose();
            nodeLine.Dispose();
            hoverNodeLine.Dispose();
            selectedNodeLine.Dispose();
        }

        public void OnLineModeDraw(ref Graphics graph)
        {
            DrawLine(ref graph);
            if (currentDrawnSector.walls.Count > 0) DrawWalls(ref graph, ref currentDrawnSector.walls, true);
        }

        public bool OnMouseDown(Point cursorPos)
        {
            currentDrawnSector.walls.Add(new Wall(DrawLineStart, DrawLineEnd, currentDrawnWallsColor, currentDrawnSector));
            DrawLineEnd = DrawLineStart = cursorPos;

            if (currentDrawnSector.walls[currentDrawnSector.walls.Count - 1].rightPoint == currentDrawnSector.walls[0].leftPoint)
            {
                MakeNewSector();
                return true;
            }

            return false;
        }

        public void SetLineEnd(Point cursorPos) { if (!DrawLineStart.IsEmpty) DrawLineEnd = cursorPos; }

        public void GetMousePos(Point mousePos) { if (mouseOnGrid) currentCursorPos = mousePos; }

        public void OnMouseEnter() => mouseOnGrid = true;
        public void OnMouseLeave() => mouseOnGrid = false;

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

        void DrawWalls(ref Graphics graph, ref List<Wall> walls, bool showNodes, int sectorIndex = -1)
        {
            int size = walls.Count;
            for (int i = 0; i < size; i++)
            {
                Pen wallPen = GetCurrentWallPen(sectorIndex, i);
                graph.DrawLine(wallPen, walls[i].leftPoint, walls[i].rightPoint);
                graph.DrawLine(wallPen, walls[i].middle, walls[i].middle.Add(walls[i].normal.Multiply(5)));

                if (showNodes)
                {
                    (SolidBrush, Pen) brushPen = GetCurrentNodeBrushAndPen(sectorIndex, i);
                    graph.FillRectangle(brushPen.Item1, new Rectangle(walls[i].leftPoint.Subtract(3), new Size(6, 6)));
                    graph.DrawRectangle(brushPen.Item2, new Rectangle(walls[i].leftPoint.Subtract(3), new Size(6, 6)));
                }
            }
        }

        public void DrawSector(ref Graphics graph, SectorDrawingData data = default)
        {
            int size = sectors.Count;
            for (int i = 0; i < size; i++)
            {
                List<Wall> walls = sectors[i].walls;
                DrawWalls(ref graph, ref walls, false, i);

                int wallsCount = walls.Count;
                Point[] wallPoints = new Point[wallsCount * 2];

                GetWallPoints(ref walls, ref wallPoints);
                graph.FillPolygon(GetSectorBrush(i), wallPoints, System.Drawing.Drawing2D.FillMode.Winding);
            }

            if (data.showWallNodes)
            {
                for (int i = 0; i < size; i++)
                {
                    int wallsCount = sectors[i].walls.Count;
                    for (int j = 0; j < wallsCount; j++)
                    {
                        (SolidBrush, Pen) brushPen = GetCurrentNodeBrushAndPen(i, j);
                        graph.FillRectangle(brushPen.Item1, new Rectangle(sectors[i].walls[j].leftPoint.Subtract(3), new Size(6, 6)));
                        graph.DrawRectangle(brushPen.Item2, new Rectangle(sectors[i].walls[j].leftPoint.Subtract(3), new Size(6, 6)));
                    }
                }
            }
        }

        (SolidBrush, Pen) GetCurrentNodeBrushAndPen(int sectorIndex, int wallIndex)
        {
            if (editor.GetCurrentSelectionType() != SelectionType.Node) return (nodeBrush, nodeLine);

            SelectionData[] currentSelection = editor.GetCurrentSelection();
            int size = currentSelection.Length;
            for (int i = 0; i < size; i++)
            {
                if (currentSelection[i].sectorIndex != sectorIndex || currentSelection[i].wallIndex != wallIndex) continue;
                return (selectedNodeBrush, selectedNodeLine);
            }

            if (CheckForNodesInPos(currentCursorPos, 5, out (int, int) indx) && indx.Item1 == sectorIndex && indx.Item2 == wallIndex)
                return (hoverNodeBrush, hoverNodeLine);
            return (nodeBrush, nodeLine);
        }

        Pen GetCurrentWallPen(int sectorIndex, int wallIndex)
        {
            if (editor.GetCurrentSelectionType() != SelectionType.Wall && editor.GetCurrentSelectionType() != SelectionType.Sector)
            {
                if (sectorIndex != -1 && wallIndex != -1)
                {
                    if (!sectors[sectorIndex].walls[wallIndex].isPortal) return wallLine;
                    return sectors[sectorIndex].walls[wallIndex].isConnection ? portalConnectionLine : portalLine;
                }

                return wallLine;
            }

            SelectionData[] currentSelection = editor.GetCurrentSelection();
            int size = currentSelection.Length;
            for (int i = 0; i < size; i++)
            {
                if (currentSelection[i].sectorIndex != sectorIndex) continue;
                if (editor.GetCurrentSelectionType() == SelectionType.Wall && currentSelection[i].wallIndex != wallIndex) continue;
                if (!sectors[sectorIndex].walls[wallIndex].isPortal) return selectedWallLine;
                return sectors[sectorIndex].walls[wallIndex].isConnection ? selectedPortalConnectionLine : selectedPortalLine;
            }

            if (editor.GetCurrentSelectionType() == SelectionType.Wall && CheckForWallsInPos(currentCursorPos, 5, out (int, int) indxs) 
                && indxs.Item1 == sectorIndex && indxs.Item2 == wallIndex)
            {
                if (!sectors[sectorIndex].walls[wallIndex].isPortal) return hoverWallLine;
                return sectors[sectorIndex].walls[wallIndex].isConnection ? hoverPortalConnectionLine : hoverPortalLine;
            }

            if (editor.GetCurrentSelectionType() == SelectionType.Sector && CheckForSectorsInPos(currentCursorPos, out int indx)
                && indx == sectorIndex)
            {
                if (!sectors[sectorIndex].walls[wallIndex].isPortal) return hoverWallLine;
                return sectors[sectorIndex].walls[wallIndex].isConnection ? hoverPortalConnectionLine : hoverPortalLine;
            }

            if (!sectors[sectorIndex].walls[wallIndex].isPortal) return wallLine;
            return sectors[sectorIndex].walls[wallIndex].isConnection ? portalConnectionLine : portalLine;
        }

        SolidBrush GetSectorBrush(int sectorIndex)
        {
            if (editor.GetCurrentSelectionType() != SelectionType.Sector) return sectorBrush;

            SelectionData[] currentSelection = editor.GetCurrentSelection();
            int size = currentSelection.Length;
            for (int i = 0; i < size; i++)
            {
                if (currentSelection[i].sectorIndex != sectorIndex) continue;
                return selectedSectorBrush;
            }

            if (CheckForSectorsInPos(currentCursorPos, out int indx) && indx == sectorIndex) return hoverSectorBrush;
            return sectorBrush;
        }

        void MakeNewSector()
        {
            Sector newSector = new Sector(currentDrawnSector);
            if (!newSector.CheckWallsOrientation()) newSector.FlipWalls();
            sectors.Add(newSector);

            int size = newSector.walls.Count, ignoreSector = sectors.Count - 1;
            for (int w = 0; w < size; w++)
            {
                bool haslftConnection = CheckPointInsideWall(newSector.walls[w].leftPoint, newSector.walls[w].normal, out (int, int) lftIndx, ignoreSector);
                bool hasRgtConnection = CheckPointInsideWall(newSector.walls[w].rightPoint, newSector.walls[w].normal, out (int, int) rgtIndx, ignoreSector);

                if (haslftConnection && hasRgtConnection) 
                {
                    if (lftIndx.Item1 == rgtIndx.Item1)
                    {
                        if (lftIndx.Item2 == rgtIndx.Item2)
                            ConnectWall_SameS_SameW(newSector.walls[w], (lftIndx.Item1, lftIndx.Item2), (ignoreSector, w));
                        else ConnectWall_SameS_DiffW(newSector.walls[w], (lftIndx.Item1, lftIndx.Item2), (ignoreSector, w));
                        continue;
                    }

                    ConnectWall_DiffS(newSector.walls[w], (lftIndx.Item1, lftIndx.Item2), (ignoreSector, w));
                }

                if (haslftConnection || hasRgtConnection)
                 ConnectWall_MissingConn(newSector.walls[w], (lftIndx.Item1, lftIndx.Item2), (ignoreSector, w));
            }

            DrawLineEnd = DrawLineStart = Point.Empty;
            currentDrawnSector.ResetDrawSector();
        }

        void ConnectWall_SameS_SameW(Wall newWall, (int, int) w1, (int, int) w2)
        {
            COLoggerImport.LogNormal("CONNECT! - Same Sector & wall");

            Wall splicedWall = sectors[w1.Item1].walls[w1.Item2];
            if ((newWall.leftPoint == splicedWall.leftPoint || newWall.leftPoint == splicedWall.rightPoint) && 
                (newWall.rightPoint == splicedWall.leftPoint || newWall.rightPoint == splicedWall.rightPoint))
            {
                sectors[w1.Item1].walls[w1.Item2] = sectors[w1.Item1].walls[w1.Item2].MakePortal(w2.Item1, w2.Item2, true);
                sectors[w2.Item1].walls[w2.Item2] = sectors[w2.Item1].walls[w2.Item2].MakePortal(w1.Item1, w1.Item2, true);
                return;
            }

            sectors[w1.Item1].walls.RemoveAt(w1.Item2);
            sectors[w2.Item1].walls[w2.Item2] = sectors[w2.Item1].walls[w2.Item2].MakePortal(w1.Item1, w1.Item2 + 1, true);
            Wall portalWall = new Wall(newWall.rightPoint, newWall.leftPoint, splicedWall.GetColors(), sectors[w1.Item1]).MakePortal(w2.Item1, w2.Item2, true);

            sectors[w1.Item1].walls.Insert(w1.Item2, new Wall(newWall.leftPoint, splicedWall.rightPoint, splicedWall.GetColors(), sectors[w1.Item1]));
            sectors[w1.Item1].walls.Insert(w1.Item2, portalWall);
            sectors[w1.Item1].walls.Insert(w1.Item2, new Wall(splicedWall.leftPoint, newWall.rightPoint, splicedWall.GetColors(), sectors[w1.Item1]));
        }

        void ConnectWall_SameS_DiffW(Wall newWall, (int, int) w1, (int, int) w2)
        {
            COLoggerImport.LogNormal("CONNECT! - Same Sector, Different Wall");

            Wall splicedWall = sectors[w1.Item1].walls[w1.Item2];
            if ((newWall.leftPoint == splicedWall.leftPoint || newWall.leftPoint == splicedWall.rightPoint) &&
                (newWall.rightPoint == splicedWall.leftPoint || newWall.rightPoint == splicedWall.rightPoint))
            {
                sectors[w1.Item1].walls[w1.Item2] = sectors[w1.Item1].walls[w1.Item2].MakePortal(w2.Item1, w2.Item2, true);
                sectors[w2.Item1].walls[w2.Item2] = sectors[w2.Item1].walls[w2.Item2].MakePortal(w1.Item1, w1.Item2, true);
                return;
            }

            sectors[w1.Item1].walls.RemoveAt(w1.Item2);
            sectors[w2.Item1].walls[w2.Item2] = sectors[w2.Item1].walls[w2.Item2].MakePortal(w1.Item1, w1.Item2 + 1, true);
            Wall portalWall = new Wall(newWall.rightPoint, newWall.leftPoint, splicedWall.GetColors(), sectors[w1.Item1]).MakePortal(w2.Item1, w2.Item2, true);

            sectors[w1.Item1].walls.Insert(w1.Item2, new Wall(newWall.leftPoint, splicedWall.rightPoint, splicedWall.GetColors(), sectors[w1.Item1]));
            sectors[w1.Item1].walls.Insert(w1.Item2, portalWall);
            sectors[w1.Item1].walls.Insert(w1.Item2, new Wall(splicedWall.leftPoint, newWall.rightPoint, splicedWall.GetColors(), sectors[w1.Item1]));
        }

        void ConnectWall_DiffS(Wall newWall, (int, int) w1, (int, int) w2)
        {
            COLoggerImport.LogNormal("CONNECT! - Different Sectors, Different Wall");
            COLoggerImport.LogWarning("Not Implemented... Skiping");
        }

        void ConnectWall_MissingConn(Wall newWall, (int, int) w1, (int, int) w2)
        {
            COLoggerImport.LogNormal("CONNECT! - Missing Connection");
            COLoggerImport.LogWarning("Not Implemented... Skiping");
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
            return new PointF(dir.Y, -dir.X);
        }

        public void LoadSectors(ref List<Sector> sectors) => this.sectors = sectors;

        public void UpdateDrawnSectorFloorHeight(int newHeight) => currentDrawnSector.floorHeight = newHeight;
        public void UpdateDrawnSectorCeillingHeight(int newHeight) => currentDrawnSector.ceillingHeight = newHeight;
        public void UpdateDrawnWallTopColor(Color color) => currentDrawnWallsColor[0] = color;
        public void UpdateDrawnWallInnerColor(Color color) => currentDrawnWallsColor[1] = color;
        public void UpdateDrawnWallBottomColor(Color color) => currentDrawnWallsColor[2] = color;
        public void UpdateDrawnSectorFloorColor(Color color) => currentDrawnSector.floorColor = color;
        public void UpdateDrawnSectorCeillingColor(Color color) => currentDrawnSector.ceillingColor = color;

        public bool CheckForNodesInPos(Point mousePoint, float dist, out (int, int) indx)
        {
            (int, int) foundIndx = (-1, -1);
            bool found = false, foundConnection = false;

            LoopSectorWalls((int i, int j, Wall wall) =>
            {
                if (foundConnection) return false;
                float lftDist = wall.leftPoint.Distance(mousePoint);

                if (lftDist <= dist)
                {
                    foundIndx.Item1 = i;
                    foundIndx.Item2 = j;
                    found = true;
                    if (wall.isPortal && wall.isConnection) foundConnection = true;
                    return true;
                }

                return false;
            }, false);

            indx = foundIndx;
            return found;
        }

        public bool CheckForWallsInPos(Point mousePoint, float dist, out (int, int) indx)
        {
            (int, int) foundIndx = (-1, -1);
            bool found = false;

            LoopSectorWalls((int i, int j, Wall wall) =>
            {
                float lftDist = wall.leftPoint.Distance(mousePoint);
                float rgtDist = wall.rightPoint.Distance(mousePoint);
                float totalDist = wall.leftPoint.Distance(wall.rightPoint);
                float sum = lftDist + rgtDist;

                if (sum - totalDist <= dist)
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

        public bool CheckForSectorsInPos(Point mousePoint, out int indx)
        {
            indx = -1;

            int sectorSize = sectors.Count;
            for (int i = 0; i < sectorSize; i++)
            {
                if (ActiveSectors[i].PointInsideSector(mousePoint)) continue;
                indx = i;
                return true;
            }

            return false;
        }

        public bool CheckPointInsideWall(Point pointToCheck, PointF normal, out (int, int) indx, int sectorToIgnore = -1)
        {
            (int, int) foundIndx = (-1, -1);
            bool found = false;

            LoopSectorWalls((int i, int j, Wall wall) =>
            {
                if (sectorToIgnore == i || wall.normal.Dot(normal) != -1) return false;

                if (wall.leftPoint == pointToCheck || wall.rightPoint == pointToCheck)
                {
                    foundIndx.Item1 = i;
                    foundIndx.Item2 = j;
                    found = true;
                    return true;
                }

                float lftDist = wall.leftPoint.Distance(pointToCheck);
                float rgtDist = wall.rightPoint.Distance(pointToCheck);
                float totalDist = wall.leftPoint.Distance(wall.rightPoint);
                float sum = lftDist + rgtDist;

                if (sum - totalDist <= .0001f)
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
                        if (!breakOrReturn) break;
                        else return;
                    }
                }
            }
        }

        public void ResetData()
        {
            currentCursorPos = Point.Empty;
            currentDrawnSector = new Sector();
            currentDrawnWallsColor[0] = currentDrawnWallsColor[1] = currentDrawnWallsColor[2] = Color.Black;
            sectors.Clear();
        }
    }
}
