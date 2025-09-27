namespace LevelEditor
{
    partial class BSPAnalyzer : Form
    {
        List<Sector> baseSectors;
        BSPBuilder bspBuilder;

        Point OriginPos => new Point(imgMainPreview.Right / 2, imgMainPreview.Bottom / 2);

        Pen wallLine, bBoxLine, splitLine, subSectorLine;
        List<SubSector> subSectors;
        BSPNode rootNode;

        public BSPAnalyzer()
        {
            InitializeComponent();

            bspBuilder = new BSPBuilder();

            wallLine = new Pen(Color.DarkGray, 2);
            bBoxLine = new Pen(Color.Red, 4);
            splitLine = new Pen(Color.Blue, 3);
            subSectorLine = new Pen(Color.Black, 3);
        }

        ~BSPAnalyzer()
        {
            wallLine.Dispose();
            bBoxLine.Dispose();
            splitLine.Dispose();
            subSectorLine.Dispose();
        }

        void ImgMainPreview_Paint(object sender, PaintEventArgs e)
        {
            Graphics graph = e.Graphics;
            DrawSector(graph);

            if (cbSubSectors.Checked)
            {
                int size = subSectors.Count;
                for (int i = 0; i < size; i++) DrawSubSectors(subSectors[i], graph);
            }

            DrawBSPNode(rootNode, graph);
        }

        private void ImgMainPreview_Resize(object sender, EventArgs e)
        {
            imgMainPreview.Invalidate();
        }

        public void InitAnalyzer(List<Sector> sectors)
        {
            baseSectors = sectors;
            rootNode = bspBuilder.PerformBSP(sectors, out subSectors);
        }

        #region Drawing

        void DrawSector(Graphics graph)
        {
            int size = baseSectors.Count;
            for (int i = 0; i < size; i++)
            {
                int wallsCount = baseSectors[i].walls.Count;
                for (int w = 0; w < wallsCount; w++)
                    graph.DrawLine(wallLine, baseSectors[i].walls[w].leftPoint.Add(OriginPos), baseSectors[i].walls[w].rightPoint.Add(OriginPos));

                Point[] wallPoints = new Point[wallsCount * 2];

                GetWallPoints(baseSectors[i].walls, ref wallPoints);
                graph.FillPolygon(Brushes.Gray, wallPoints, System.Drawing.Drawing2D.FillMode.Winding);
            }
        }

        void DrawBSPNode(BSPNode node, Graphics graph)
        {
            if (node == null) return;

            if (cbBBox.Checked) DrawBBox(ref node.bBox, graph);
            if (cbSplitters.Checked) DrawSplitters(ref node.splitter, graph);

            DrawBSPNode(node.frontNode, graph);
            DrawBSPNode(node.backNode, graph);
        }

        void DrawSubSectors(SubSector subSector, Graphics graph)
        {
            if (subSector == null) return;
            subSectorLine.Color = RandomColor();

            int wallCount = subSector.walls.Count;
            for (int w = 0; w < wallCount; w++)
                graph.DrawLine(subSectorLine, subSector.walls[w].leftPoint.Add(OriginPos), subSector.walls[w].rightPoint.Add(OriginPos));
        }

        void DrawSplitters(ref Splitter splitter, Graphics graph)
        {
            if (splitter == Splitter.Empty) return;
            Point startPoint = splitter.startPoint.Add(OriginPos);
            graph.DrawLine(splitLine, startPoint, startPoint.Add(splitter.segment));
        }

        void DrawBBox(ref BoundingBox bbox, Graphics graph)
        {
            Point leftTopPoint = bbox.topPoint.Add(OriginPos);
            Point rightTopPoint = new Point(bbox.bottomPoint.X, bbox.topPoint.Y).Add(OriginPos);
            Point rightBottomPoint = bbox.bottomPoint.Add(OriginPos);
            Point leftBottomPoint = new Point(bbox.topPoint.X, bbox.bottomPoint.Y).Add(OriginPos);

            graph.DrawLine(bBoxLine, leftTopPoint, rightTopPoint);
            graph.DrawLine(bBoxLine, rightTopPoint, rightBottomPoint);
            graph.DrawLine(bBoxLine, rightBottomPoint, leftBottomPoint);
            graph.DrawLine(bBoxLine, leftBottomPoint, leftTopPoint);
        }

        void GetWallPoints(List<Wall> walls, ref Point[] wallPoints)
        {
            int count = wallPoints.Length;
            for (int j = 0, n = 0; n < count; j++, n++)
            {
                wallPoints[n++] = walls[j].leftPoint.Add(OriginPos);
                wallPoints[n] = walls[j].rightPoint.Add(OriginPos);
            }
        }

        static Color RandomColor()
        {
            int r = Random.Shared.Next(0, 255);
            int g = Random.Shared.Next(0, 255);
            int b = Random.Shared.Next(0, 255);
            return Color.FromArgb(r, g, b);
        }

        #endregion

        void OnCheckBoxChanged(object sender, EventArgs e) => imgMainPreview.Invalidate();
    }
}
