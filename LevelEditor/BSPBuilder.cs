namespace LevelEditor
{
    internal class BSPNode
    {
        public Wall wall;
        public int sectorIndex;
        public List<BSPNode> frontNodes;
        public List<BSPNode> backNodes;

        public BSPNode(int sectorIndx, Wall wall)
        {
            sectorIndex = sectorIndx;
            this.wall = wall;
            frontNodes = new List<BSPNode>();
            backNodes = new List<BSPNode>();
        }
    }

    internal class BSPBuilder
    {
        public int debug_NumberOfIntersections;

        public BSPNode PerformBSP(List<Sector> sectors, out Wall splitterSegment)
        {
            Queue<Wall> walls = new Queue<Wall>();

            int sectorSize = sectors.Count;
            for (int i = 0; i < sectorSize; i++)
            {
                int wallsSize = sectors[i].walls.Count;
                for (int j = 0; j < wallsSize; j++)
                    walls.Enqueue(sectors[i].walls[j]);
            }

            splitterSegment = new Wall(walls.Peek());

            BSPNode bsp = new BSPNode(0, walls.Dequeue());
            PartitionSector(bsp, ref splitterSegment, ref walls, ref sectors);
            return bsp;
        }

        void PartitionSector(BSPNode rootNode, ref Wall splitter, ref Queue<Wall> walls, ref List<Sector> sectors)
        {
            while (walls.Count > 0)
            {
                Wall wall = walls.Dequeue();
                FindWallSectorByID(wall.wallID, ref sectors, out int wallSector);
                PartitionWall(splitter, rootNode, ref wall, wallSector);
            }

            CheckChildNodes(splitter, ref rootNode.frontNodes, ref sectors);
            CheckChildNodes(splitter, ref rootNode.backNodes, ref sectors);
        }

        void CheckChildNodes(Wall splitter, ref List<BSPNode> nodeList, ref List<Sector> sectors)
        {
            if (nodeList.Count <= 1) return;

            while (nodeList.Count > 1)
            {
                Wall wall = nodeList[1].wall;
                FindWallSectorByID(wall.wallID, ref sectors, out int wallSector);
                PartitionWall(splitter, nodeList[0], ref wall, wallSector);
                nodeList.RemoveAt(1);
            }

            CheckChildNodes(splitter, ref nodeList[0].frontNodes, ref sectors);
            CheckChildNodes(splitter, ref nodeList[0].backNodes, ref sectors);
        }

        void PartitionWall(Wall splitter, BSPNode currentNode, ref Wall currentWall, int wallSector)
        {
            Point ab = splitter.rightPoint.Subtract(splitter.leftPoint);
            Point ac = currentWall.rightPoint.Subtract(splitter.leftPoint);
            Point dc = currentWall.rightPoint.Subtract(currentWall.leftPoint);

            int num = ac.Cross(ab);
            int den = ab.Cross(dc);

            bool numIsZero = MathF.Abs(num) == 0;
            bool denIsZero = MathF.Abs(den) == 0;

            if (numIsZero && denIsZero)
            {
                currentNode.frontNodes.Add(new BSPNode(wallSector, currentWall));
                return;
            }

            if (!denIsZero)
            {
                float intersection = num / den;
                if (intersection > 0 && intersection < 1)
                {
                    Point center = currentWall.leftPoint.Add(dc.Multiply(intersection));
                    Wall aiWall = new Wall(currentWall.leftPoint, center, currentWall);
                    Wall ibWall = new Wall(center, currentWall.rightPoint, currentWall);

                    if (num > 0)
                    {
                        Wall temp = aiWall;
                        aiWall = ibWall;
                        ibWall = temp;
                    }

                    currentNode.frontNodes.Add(new BSPNode(wallSector, aiWall));
                    currentNode.backNodes.Add(new BSPNode(wallSector, ibWall));
                    debug_NumberOfIntersections++;
                    return;
                }    
            }

            if (num < 0 || (numIsZero && den > 0)) currentNode.frontNodes.Add(new BSPNode(wallSector, currentWall));
            else if (num > 0 || (numIsZero && den < 0)) currentNode.backNodes.Add(new BSPNode(wallSector, currentWall));
        }

        bool FindWallSectorByID(ulong id, ref List<Sector> sectors, out int sector)
        {
            sector = -1;

            int sectorsSize = sectors.Count;
            for (int s = 0; s < sectorsSize; s++)
            {
                int wallsSize = sectors[s].walls.Count;
                for (int w = 0; w < wallsSize; w++)
                {
                    if (sectors[s].walls[w].wallID != id) continue;
                    sector = s;
                    return true;
                }
            }

            return false;
        }
    }
}
