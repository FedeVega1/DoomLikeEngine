namespace LevelEditor
{
    partial class BSPAnalyzer : Form
    {
        //enum BSPChildPhase { Front, Back }

        //struct BSPRecord
        //{
        //    public Wall segment;
        //    public Color color;
        //    public int parentNodeIndex, currentNodeIndex;
        //}

        //List<Sector> baseSectors;

        //Point OriginPos => new Point(imgMainPreview.Right / 2, imgMainPreview.Bottom / 2);

        //Pen wallLine, bspLine;
        //System.Windows.Forms.Timer bspTimer;
        //BSPNode rootNodeTree;
        //List<BSPRecord> lastRecordList, currentRecordList;
        //int timesize;

        public BSPAnalyzer()
        {
            InitializeComponent();

            //wallLine = new Pen(Color.DarkGray, 2);
            //bspLine = new Pen(Color.LightGreen, 3);

            //lastRecordList = new List<BSPRecord>();
            //currentRecordList = new List<BSPRecord>();

            //bspTimer = new System.Windows.Forms.Timer();
        }

        //~BSPAnalyzer()
        //{
        //    wallLine.Dispose();
        //    bspLine.Dispose();
        //}

        void ImgMainPreview_Paint(object sender, PaintEventArgs e)
        {
            Graphics graph = e.Graphics;
            //DrawSector(ref graph);
            //DrawRecordList(ref graph);
            //DrawBSPTree(ref graph);
        }

        private void ImgMainPreview_Resize(object sender, EventArgs e)
        {
            imgMainPreview.Invalidate();
        }

        public void InitAnalyzer(List<Sector> sectors)
        {
            //baseSectors = sectors;

            //List<Wall> walls = new List<Wall>();

            //int sectorSize = sectors.Count;
            //for (int i = 0; i < sectorSize; i++)
            //{
            //    int wallsSize = sectors[i].walls.Count;
            //    for (int j = 0; j < wallsSize; j++)
            //        walls.Add(sectors[i].walls[j]);
            //}

            //timesize = 0;
            //currentRecordList.Clear();
            //lastRecordList.Clear();

            //lblCurrentNode.Text = "Current Node: Root";
            //lblProcess.Text = "Start!";
            //lblPartition.Text = "";

            //rootNodeTree = new BSPNode(walls, walls[0], null);
            //GenerateLeafs(rootNodeTree);

            //bspTimer.Interval = 1000;
            //bspTimer.Tick += BSPTimer1_Tick;
            //bspTimer.Start();
        }

        //void BSPTimer1_Tick(object sender, EventArgs e)
        //{
        //    timesize = Math.Clamp(timesize + 1, 0, lastRecordList.Count);
        //    lblCurrentNode.Text = string.Format("Current Node: {0}/{1}", lastRecordList[timesize - 1].parentNodeIndex, lastRecordList[timesize - 1].currentNodeIndex);
        //    lblProcess.Text = timesize == lastRecordList.Count ? "Finished" : "In Process";
        //    imgMainPreview.Invalidate();
        //}

        //static Color RandomColor()
        //{
        //    int r = Random.Shared.Next(0, 255);
        //    int g = Random.Shared.Next(0, 255);
        //    int b = Random.Shared.Next(0, 255);
        //    return Color.FromArgb(r, g, b);
        //}

        //#region Drawing

        //void DrawBSPTree(ref Graphics graph)
        //{
        //    Rectangle startingRectangle = Rectangle.FromLTRB(OriginPos.X - 20, 5, OriginPos.X + 25, 20);
        //    graph.DrawRectangle(Pens.White, startingRectangle);

        //    DrawBSPChild(ref graph, rootNodeTree, rootNodeTree.frontNode, startingRectangle, true);
        //    DrawBSPChild(ref graph, rootNodeTree, rootNodeTree.backNode, startingRectangle, false);
        //}

        //void DrawBSPChild(ref Graphics graph, BSPNode currentNode, BSPNode child, Rectangle rect, bool frontBack)
        //{
        //    if (child == null) return;
        //    Rectangle newRect;

        //    int extraSep = 0;
        //    if (currentNode.frontNode != null && currentNode.backNode != null) extraSep = 300;

        //    if (frontBack)
        //    {
        //        newRect = Rectangle.FromLTRB(rect.Left + 100 + extraSep, rect.Top + 100, rect.Right + 100 + extraSep, rect.Bottom + 100);
        //        graph.DrawRectangle(Pens.White, newRect);

        //        Point point1 = new Point(rect.Left + ((rect.Right - rect.Left) / 2), rect.Bottom);
        //        Point point2 = new Point(newRect.Left + ((newRect.Right - newRect.Left) / 2), newRect.Top);
        //        graph.DrawLine(Pens.Green, point1, point2);
        //    }
        //    else
        //    {
        //        newRect = Rectangle.FromLTRB(rect.Left - 100 - extraSep, rect.Top + 100, rect.Right - 100 - extraSep, rect.Bottom + 100);
        //        graph.DrawRectangle(Pens.White, newRect);

        //        Point point1 = new Point(rect.Left + ((rect.Right - rect.Left) / 2), rect.Bottom);
        //        Point point2 = new Point(newRect.Left + ((newRect.Right - newRect.Left) / 2), newRect.Top);
        //        graph.DrawLine(Pens.Red, point1, point2);
        //    }

        //    DrawBSPChild(ref graph, child, child.frontNode, newRect, true);
        //    DrawBSPChild(ref graph, child, child.backNode, newRect, false);
        //}

        //void DrawRecordList(ref Graphics graph)
        //{
        //    for (int i = 0; i < timesize; i++)
        //    {
        //        bspLine.Color = lastRecordList[i].color;
        //        graph.DrawLine(bspLine, lastRecordList[i].segment.leftPoint.Add(OriginPos), lastRecordList[i].segment.rightPoint.Add(OriginPos));
        //    }
        //}

        //void DrawSector(ref Graphics graph)
        //{
        //    int size = baseSectors.Count;
        //    for (int i = 0; i < size; i++)
        //    {
        //        int wallsCount = baseSectors[i].walls.Count;
        //        for (int w = 0; w < wallsCount; w++)
        //            graph.DrawLine(wallLine, baseSectors[i].walls[w].leftPoint.Add(OriginPos), baseSectors[i].walls[w].rightPoint.Add(OriginPos));

        //        Point[] wallPoints = new Point[wallsCount * 2];

        //        GetWallPoints(baseSectors[i].walls, ref wallPoints);
        //        graph.FillPolygon(Brushes.Gray, wallPoints, System.Drawing.Drawing2D.FillMode.Winding);
        //    }
        //}

        //void GetWallPoints(List<Wall> walls, ref Point[] wallPoints)
        //{
        //    int count = wallPoints.Length;
        //    for (int j = 0, n = 0; n < count; j++, n++)
        //    {
        //        wallPoints[n++] = walls[j].leftPoint.Add(OriginPos);
        //        wallPoints[n] = walls[j].rightPoint.Add(OriginPos);
        //    }
        //}

        //#endregion

        //#region BSP Code

        //void GenerateLeafs(BSPNode node)
        //{
        //    if (node == null) return;

        //    int size = node.walls.Count;
        //    System.Diagnostics.Debug.Assert(size > 0);
        //    if (size == 1) return;

        //    Wall splitter = node.walls[0];
        //    while (node.walls.Count > 1)
        //    {
        //        Wall wall = node.walls[1];
        //        PartitionWall(node, ref splitter, ref wall);
        //        node.walls.RemoveAt(1);
        //    }

        //    GenerateLeafs(node.frontNode);
        //    GenerateLeafs(node.backNode);
        //}

        //void PartitionWall(BSPNode currentNode, ref Wall splitter, ref Wall currentWall)
        //{
        //    Point ab = splitter.rightPoint.Subtract(splitter.leftPoint);
        //    Point ac = currentWall.rightPoint.Subtract(splitter.leftPoint);
        //    Point dc = currentWall.rightPoint.Subtract(currentWall.leftPoint);

        //    int num = ac.Cross(ab);
        //    int den = ab.Cross(dc);

        //    bool numIsZero = MathF.Abs(num) == 0;
        //    bool denIsZero = MathF.Abs(den) == 0;

        //    if (numIsZero && denIsZero)
        //    {
        //        if (currentNode.frontNode == null) currentNode.frontNode = new BSPNode(currentWall, splitter, currentNode);
        //        else currentNode.frontNode.walls.Add(currentWall);
        //        return;
        //    }

        //    if (!denIsZero)
        //    {
        //        float intersection = num / den;
        //        if (intersection > 0 && intersection < 1)
        //        {
        //            Point center = currentWall.leftPoint.Add(dc.Multiply(intersection));
        //            Wall aiWall = new Wall(currentWall.leftPoint, center, currentWall);
        //            Wall ibWall = new Wall(center, currentWall.rightPoint, currentWall);

        //            if (num > 0)
        //            {
        //                Wall temp = aiWall;
        //                aiWall = ibWall;
        //                ibWall = temp;
        //            }

        //            if (currentNode.frontNode == null) currentNode.frontNode = new BSPNode(aiWall, splitter, currentNode);
        //            else currentNode.frontNode.walls.Add(currentWall);

        //            if (currentNode.backNode == null) currentNode.backNode = new BSPNode(ibWall, splitter, currentNode);
        //            else currentNode.backNode.walls.Add(currentWall);
        //            return;
        //        }
        //    }

        //    if (num < 0 || (numIsZero && den > 0))
        //    {
        //        if (currentNode.frontNode == null) currentNode.frontNode = new BSPNode(currentWall, splitter, currentNode);
        //        else currentNode.frontNode.walls.Add(currentWall);
        //        return;
        //    }

        //    if (num > 0 || (numIsZero && den < 0))
        //    {
        //        if (currentNode.backNode == null) currentNode.backNode = new BSPNode(currentWall, splitter, currentNode);
        //        else currentNode.backNode.walls.Add(currentWall);
        //    }
        //}

        //#endregion
    }
}
