namespace LevelEditor
{
    public partial class MainWindow : Form
    {
        MapGridEditor gridEditor;

        public MainWindow()
        {
            InitializeComponent();
            COLoggerImport.InitLogSys(true, false);

            gridEditor = new MapGridEditor(new GridEditorData(ref LblCursor, ref LblOrigin, ref LblGridSize, ref ImgEditorDraw));
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
                    gridEditor.ToggleLineMode(false);
                    return true;

                case Keys.Up: gridEditor.MoveOrigin(new PointF(0, -1)); return true;
                case Keys.Down: gridEditor.MoveOrigin(new PointF(0, 1)); return true;
                case Keys.Left: gridEditor.MoveOrigin(new PointF(-1, 0)); return true;
                case Keys.Right: gridEditor.MoveOrigin(new PointF(1, 0)); return true;
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
    }
}
