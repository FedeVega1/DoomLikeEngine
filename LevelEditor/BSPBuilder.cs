namespace LevelEditor
{
    internal class SubSector
    {
        public static uint numberOfSubSectors;

        public uint SubSectorID { get; private set; }

        public List<Wall> walls;

        public SubSector(IList<Wall> ws)
        {
            SubSectorID = numberOfSubSectors++;
            walls = new List<Wall>(ws);
        }
    }

    internal struct Splitter
    {
        public static Splitter Empty = new Splitter();

        public Point startPoint;
        public Point segment;
        public PointF dir;

        public Splitter(Point start, Point s, PointF d)
        {
            startPoint = start;
            segment = s;
            dir = d;
        }

        public Splitter(Wall wall)
        {
            startPoint = wall.leftPoint;
            segment = wall.leftPoint.Subtract(wall.rightPoint);
            dir = segment.Normalize();
        }

        public static bool operator ==(Splitter split1, Splitter split2) => split1.Equals(split2);
        public static bool operator !=(Splitter split1, Splitter split2) => !split1.Equals(split2);

        public override bool Equals(object obj)
        {
            if (obj is not Splitter) return false;
            Splitter other = (Splitter) obj;
            return other.startPoint == startPoint && other.segment == segment;
        }

        public override int GetHashCode() => base.GetHashCode();
    }

    internal struct BoundingBox
    {
        public Point topPoint, bottomPoint;
        public PointF center;

        public BoundingBox(Point tPoint, Point bPoint)
        {
            topPoint = tPoint;
            bottomPoint = bPoint;
            center = new PointF((topPoint.X - bottomPoint.X) / 2f, (bottomPoint.Y - topPoint.Y) / 2f);
        }

        public BoundingBox(IList<Wall> wallList)
        {
            int size = wallList.Count;
            for (int i = 0; i < size; i++)
                ExpandBox(wallList[i]);
        }

        public static bool operator==(BoundingBox bbox1, BoundingBox bbox2) => bbox1.Equals(bbox2);
        public static bool operator!=(BoundingBox bbox1, BoundingBox bbox2) => !bbox1.Equals(bbox2);

        public override bool Equals(object obj)
        {
            if (obj is not BoundingBox) return false;
            BoundingBox other = (BoundingBox) obj;
            return other.topPoint == topPoint && other.bottomPoint == bottomPoint;
        }

        public override int GetHashCode() => base.GetHashCode();

        public void ExpandBox(Wall wall)
        {
            if (!PointInsideBox(ref wall.leftPoint)) ExpandPoint(wall.leftPoint);
            if (!PointInsideBox(ref wall.rightPoint)) ExpandPoint(wall.rightPoint);
            center = new Point((topPoint.X - bottomPoint.X) / 2, (bottomPoint.Y - topPoint.Y) / 2);
        }

        public readonly int IsInsideBox(Wall wall)
        {
            bool leftPoint = PointInsideBox(ref wall.leftPoint);
            bool rightPoint = PointInsideBox(ref wall.rightPoint);

            if (leftPoint && rightPoint) return 1;
            if (leftPoint || rightPoint) return 0;
            return -1;
        }

        readonly bool PointInsideBox(ref Point point)
        {
            bool top = point.X > topPoint.X && point.Y < topPoint.Y;
            bool btm = point.X < bottomPoint.X && point.Y > bottomPoint.Y;
            return top && btm;
        }

        void ExpandPoint(Point other)
        {
            if (other.X < topPoint.X) topPoint.X -= Math.Abs(topPoint.X - other.X);
            if (other.Y > topPoint.Y) topPoint.Y += Math.Abs(topPoint.Y - other.Y);

            if (other.X > bottomPoint.X) bottomPoint.X += Math.Abs(bottomPoint.X - other.X);
            if (other.Y < bottomPoint.Y) bottomPoint.Y -= Math.Abs(bottomPoint.Y - other.Y);
        }

        public readonly bool WallCutsBox(Wall wall)
        {
            if (wall.leftPoint.Y == wall.rightPoint.Y)
                return wall.leftPoint.Y != topPoint.Y && wall.leftPoint.Y != bottomPoint.Y;

            if (wall.leftPoint.X == wall.rightPoint.X)
                return wall.leftPoint.X != topPoint.X && wall.leftPoint.X != bottomPoint.X;

            return true;
        }
    }

    internal class BSPNode
    {
        public static uint numberOfNodes;

        public uint NodeID { get; private set; }

        public BoundingBox bBox;
        public SubSector subSector;
        public Splitter splitter;
        public List<Wall> walls;
        public BSPNode frontNode, backNode, parentNode;

        public BSPNode(IList<Wall> ws, BoundingBox box, Splitter split, BSPNode parent)
        {
            NodeID = numberOfNodes++;
            walls = new List<Wall>(ws);
            bBox = box;
            splitter = split;
            frontNode = null;
            backNode = null;
            parentNode = parent;
        }

        public BSPNode(Wall ws, BSPNode parent)
        {
            NodeID = numberOfNodes++;
            walls = new List<Wall> { ws };
            frontNode = null;
            backNode = null;
            parentNode = parent;
        }

        public bool IsConvex()
        {
            List<Point> points = GetOrderedPointList(walls);

            if (points.Count < walls.Count) return false;

            int size = points.Count;
            for (int i = 0; i < size; i++)
            {
                Point a = points[i];
                Point b = points[(i + 1) % size];
                Point c = points[(i + 2) % size];

                int cross = ZCrossProduct(a, b, c);
                if (cross > 0) return false;
            }

            return true;
        }

        static List<Point> GetOrderedPointList(List<Wall> walls)
        {
            List<Point> pointList = new List<Point>();
            if (walls.Count == 0) return pointList;

            pointList.Add(walls[0].leftPoint);
            Point currentPoint = walls[0].rightPoint;
            pointList.Add(currentPoint);

            HashSet<Wall> wallSet = new HashSet<Wall>(walls.Skip(1));

            while (wallSet.Count > 0)
            {
                Wall wall = wallSet.FirstOrDefault(wall => wall.leftPoint == currentPoint);
                if (wall == null) break;

                currentPoint = wall.rightPoint;
                pointList.Add(currentPoint);
                wallSet.Remove(wall);
            }

            return pointList;
        }

        static int ZCrossProduct(Point A, Point B, Point C)
        {
            Point d1 = new Point(B.X - A.X, B.Y - A.Y);
            Point d2 = new Point(C.X - B.X, C.Y - B.Y);
            return (d1.X * d2.Y) - (d1.Y * d2.X);
        }
    }

    internal class BSPBuilder
    {
        enum SimulationResult { None, TotalFront, TotalBack, Split, Mix }

        public int debug_NumberOfIntersections;

        int[] simResults;
        List<SubSector> currentSubSectors;
        List<(Splitter, SimulationResult, int, int)> splitterResults;
        Point[] directions;

        public BSPBuilder()
        {
            simResults = new int[3] { 0, 0, 0 };
            splitterResults = new List<(Splitter, SimulationResult, int, int)>();

            directions = new Point[6]
            {
                new Point(1, 0), new Point(-1, 0),
                new Point(0, 1), new Point(0, -1),
                new Point(1, 1), new Point(-1, -1),
            };

            currentSubSectors = new List<SubSector>();
        }

        public BSPNode PerformBSP(List<Sector> sectors, out List<SubSector> subSectors)
        {
            List<Wall> walls = new List<Wall>();
            BoundingBox rootBox = new BoundingBox();

            int sectorSize = sectors.Count;
            for (int i = 0; i < sectorSize; i++)
            {
                int wallsSize = sectors[i].walls.Count;
                for (int j = 0; j < wallsSize; j++)
                {
                    walls.Add(sectors[i].walls[j]);
                    rootBox.ExpandBox(sectors[i].walls[j]);
                }
            }

            BSPNode bsp = new BSPNode(walls, rootBox, SelectASplitter(walls, ref rootBox), null);
            GenerateLeafs(bsp);

            subSectors = new List<SubSector>(currentSubSectors);
            currentSubSectors.Clear();

            return bsp;
        }

        void GenerateLeafs(BSPNode node)
        {
            if (node == null) return;

            //if (node.parentNode != null)
            //{
            //    System.Diagnostics.Debug.Assert(node.splitter != node.parentNode.splitter);
            //    System.Diagnostics.Debug.Assert(node.bBox != node.parentNode.bBox);
            //}

            if (node.IsConvex())
            {
                SubSector subSector = new SubSector(node.walls);
                currentSubSectors.Add(subSector);
                node.subSector = subSector;
                return;
            }

            int size = node.walls.Count;
            for (int i = 0; i < size; i++) PartitionWall(node, node.walls[i]);

            if (node.frontNode != null)
            {
                node.frontNode.bBox = new BoundingBox(node.frontNode.walls);
                node.frontNode.splitter = SelectASplitter(node.frontNode.walls, ref node.bBox);
                GenerateLeafs(node.frontNode);
            }

            if (node.backNode != null)
            {
                node.backNode.bBox = new BoundingBox(node.backNode.walls);
                node.backNode.splitter = SelectASplitter(node.backNode.walls, ref node.bBox);
                GenerateLeafs(node.backNode);
            }
        }

        void PartitionWall(BSPNode currentNode, Wall currentWall)
        {
            Point ab = currentNode.splitter.segment;
            Point ca = currentWall.leftPoint.Subtract(currentNode.splitter.startPoint);
            Point da = currentWall.rightPoint.Subtract(currentNode.splitter.startPoint);

            int num = ab.Cross(ca), den = ab.Cross(da);
            bool numIsZero = Math.Abs(num) == 0, denIsZero = Math.Abs(den) == 0;

            if ((numIsZero && denIsZero) || (num >= 0 && den >= 0))
            {
                if (currentNode.frontNode == null) currentNode.frontNode = new BSPNode(currentWall, currentNode);
                else currentNode.frontNode.walls.Add(currentWall);
                return;
            }

            if (num <= 0 && den <= 0)
            {
                if (currentNode.backNode == null) currentNode.backNode = new BSPNode(currentWall, currentNode);
                else currentNode.backNode.walls.Add(currentWall);
                return;
            }

            Point cd = currentWall.leftPoint.Subtract(currentNode.splitter.startPoint);
            int cross = ab.Cross(cd);

            if (Math.Abs(cross) == 0)
            {
                if (currentNode.frontNode == null) currentNode.frontNode = new BSPNode(currentWall, currentNode);
                else currentNode.frontNode.walls.Add(currentWall);
                return;
            }

            Point intersection = currentWall.leftPoint.Add(cd.Multiply(num / cross));
            Wall ciWall = new Wall(currentWall.leftPoint, intersection, currentWall);
            Wall idWall = new Wall(intersection, currentWall.rightPoint, currentWall);

            if (currentNode.frontNode == null) currentNode.frontNode = new BSPNode(ciWall, currentNode);
            else currentNode.frontNode.walls.Add(ciWall);

            if (currentNode.backNode == null) currentNode.backNode = new BSPNode(idWall, currentNode);
            else currentNode.backNode.walls.Add(idWall);
            
            debug_NumberOfIntersections++;
        }

        Splitter SelectASplitter(List<Wall> walls, ref BoundingBox bBox)
        {
            AnalyzeWalls(walls);

            Splitter bestSplitter = CheckSimulationResults();
            if (bestSplitter != Splitter.Empty) return bestSplitter;

            splitterResults.Clear();
            Point center = new Point((int) MathF.Round(bBox.center.X), (int) MathF.Round(bBox.center.Y));

            for (int i = 0; i < 6; i++)
            {
                ClearSimResults();
                Splitter splitter = new Splitter(center, directions[i].Multiply(30), directions[i]);

                SimulateSplits(ref splitter, walls, ref simResults);
                AnalyzeSimResults(ref splitter);
            }

            return CheckSimulationResults();
        }

        void AnalyzeWalls(List<Wall> walls)
        {
            splitterResults.Clear();

            int size = walls.Count;
            for (int i = 0; i < size; i++)
            {
                ClearSimResults();
                Splitter splitter = new Splitter(walls[i]);

                SimulateSplits(ref splitter, walls, ref simResults);
                AnalyzeSimResults(ref splitter);
            }
        }

        void AnalyzeSimResults(ref Splitter splitter)
        {
            (Splitter, SimulationResult, int, int) res;
            if (simResults[2] > 0) res = (splitter, SimulationResult.Split, simResults[2], 0);
            else if (simResults[0] > 0 && simResults[1] > 0) res = (splitter, SimulationResult.Mix, simResults[0], simResults[1]);
            else if (simResults[0] > 0 && simResults[1] == 0) res = (splitter, SimulationResult.TotalFront, simResults[0], 0);
            else res = (splitter, SimulationResult.TotalBack, simResults[1], 0);

            splitterResults.Add(res);
        }

        void ClearSimResults() { for (int i = 0; i < 3; i++) simResults[i] = 0; }

        Splitter CheckSimulationResults()
        {
            Splitter bestSplitter = new Splitter();
            SimulationResult lastResult = SimulationResult.None;
            float lastResultValues = 0;

            int size = splitterResults.Count;
            for (int i = 0; i < size; i++)
            {
                if (splitterResults[i].Item2 < SimulationResult.Split || splitterResults[i].Item2 < lastResult) continue;

                bool isBestSplitter = false;
                switch (splitterResults[i].Item2)
                {
                    case SimulationResult.Split:
                        if (splitterResults[i].Item2 != lastResult || splitterResults[i].Item3 < lastResultValues)
                        {
                            lastResultValues = splitterResults[i].Item3;
                            isBestSplitter = true;
                        }
                        break;

                    case SimulationResult.Mix:
                        float diff = ((float) splitterResults[i].Item3 / (float) splitterResults[i].Item4) / .5f;
                        if (splitterResults[i].Item2 != lastResult || Math.Abs(diff - 1) < Math.Abs(lastResultValues - 1))
                        {
                            lastResultValues = diff;
                            isBestSplitter = true;
                        }
                        break;
                }

                if (!isBestSplitter) continue;
                bestSplitter = splitterResults[i].Item1;
                lastResult = splitterResults[i].Item2;
            }

            return bestSplitter;
        }

        static void SimulateSplits(ref Splitter splitter, List<Wall> wallList, ref int[] result)
        {
            if (result.Length < 3) return;

            int size = wallList.Count;
            for (int i = 0; i < size; i++)
            {
                Point ab = splitter.segment;
                Point ca = wallList[i].leftPoint.Subtract(splitter.startPoint);
                Point da = wallList[i].rightPoint.Subtract(splitter.startPoint);

                int num = ab.Cross(ca), den = ab.Cross(da);
                bool numIsZero = Math.Abs(num) == 0, denIsZero = Math.Abs(den) == 0;

                if ((numIsZero && denIsZero) || (num >= 0 && den >= 0))
                {
                    result[0]++;
                    continue;
                }

                if (num <= 0 && den <= 0) { result[1]++; continue; }

                Point cd = wallList[i].leftPoint.Subtract(splitter.startPoint);
                if (Math.Abs(ab.Cross(cd)) == 0) { result[0]++; continue; }
                result[2]++;
            }
        }
    }
}
