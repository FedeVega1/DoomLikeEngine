namespace LevelEditor
{
    public partial class MainWindow : Form
    {
        MapGridEditor gridEditor;
        FileManager fileManager;

        public MainWindow()
        {
            InitializeComponent();
            COLoggerImport.InitLogSys(true, false);

            gridEditor = new MapGridEditor(new GridEditorData(ref LblCursor, ref LblOrigin, ref LblGridSize, ref ImgEditorDraw));
            fileManager = new FileManager();
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            switch (keyData)
            {
                case Keys.Add:
                    gridEditor.UpdateGridSize(1);
                    return true;

                case Keys.Subtract:
                    gridEditor.UpdateGridSize(-1);
                    return true;

                case Keys.Escape:
                    if (!gridEditor.IsDrawingLine) gridEditor.ToggleLineMode(false);
                    else gridEditor.ToggleLineDrawMode(false);
                    return true;

                case Keys.Up: gridEditor.MoveOrigin(new PointF(0, 1)); return true;
                case Keys.Down: gridEditor.MoveOrigin(new PointF(0, -1)); return true;
                case Keys.Left: gridEditor.MoveOrigin(new PointF(1, 0)); return true;
                case Keys.Right: gridEditor.MoveOrigin(new PointF(-1, 0)); return true;
            }

            return base.ProcessCmdKey(ref msg, keyData);
        }

        void ImgEditorDraw_Paint(object sender, PaintEventArgs e)
        {
            Graphics graph = e.Graphics;
            gridEditor.OnPaint(ref graph);
        }

        void ImgEditorDraw_MouseDown(object sender, MouseEventArgs e) => gridEditor.OnMouseDown(e.Button, e.Location);

        void ImgEditorDraw_MouseMove(object sender, MouseEventArgs e) => gridEditor.OnMouseMove(e.Location);

        void ImgEditorDraw_MouseUp(object sender, MouseEventArgs e) => gridEditor.OnMouseUp(e.Button);

        void ImgEditorDraw_MouseEnter(object sender, EventArgs e) => gridEditor.OnMouseEnter();

        void ImgEditorDraw_MouseLeave(object sender, EventArgs e) => gridEditor.OnMouseLeave();

        void BtnLines_Click(object sender, EventArgs e) => gridEditor.ToggleLineMode(true);

        void BtnFileNew_Click(object sender, EventArgs e) => gridEditor.ResetData();

        void BtnFileLoad_Click(object sender, EventArgs e) => OpenFilePanel.ShowDialog();

        void BtnFileSave_Click(object sender, EventArgs e) => SaveFilePanel.ShowDialog();

        void BtnEditUndo_Click(object sender, EventArgs e)
        {

        }

        void BtnEditRedo_Click(object sender, EventArgs e)
        {

        }

        void OpenFilePanel_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            using (Stream fileStream = OpenFilePanel.OpenFile())
            {
                gridEditor.ResetData();
                fileManager.LoadFromFile(fileStream, out List<Sector> sectors);
                gridEditor.LoadSectors(sectors);
            }
        }

        void SaveFilePanel_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            gridEditor.GetSectors(out List<Sector> sectors);
            using (Stream fileStream = SaveFilePanel.OpenFile())
                fileManager.SaveToFile(fileStream, ref sectors);
        }
    }
}
