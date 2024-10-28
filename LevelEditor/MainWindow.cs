namespace LevelEditor
{
    public partial class MainWindow : Form
    {
        enum ColorPanelCallers { None, Wall, Floor, Ceilling }

        MapGridEditor gridEditor;
        FileManager fileManager;
        AboutWindow about;

        ColorPanelCallers currentColorPanelCaller;
        System.Windows.Forms.Timer listenToColorPanelTimer;

        public MainWindow()
        {
            InitializeComponent();
            Text = "Level Editor - NewProject.map";
            SaveFilePanel.FileName = "NewProject";

            WindowState = FormWindowState.Maximized;
            listenToColorPanelTimer = new System.Windows.Forms.Timer();

            if (NumbCeillingHeight.NumericUpDownControl != null)
            {
                NumbCeillingHeight.NumericUpDownControl.Maximum = int.MaxValue;
                NumbCeillingHeight.NumericUpDownControl.Minimum = int.MinValue;
            }

            if (NumbFloorHeight.NumericUpDownControl != null)
            {
                NumbFloorHeight.NumericUpDownControl.Maximum = int.MaxValue;
                NumbFloorHeight.NumericUpDownControl.Minimum = int.MinValue;
            }

            about = new AboutWindow();
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

        void BtnLinesMode_Click(object sender, EventArgs e) => gridEditor.ToggleLineMode(true);

        void BtnFileNew_Click(object sender, EventArgs e)
        {
            fileManager.ResetCurrentFile();
            Text = "Level Editor - NewProject.map";

            if (NumbCeillingHeight.NumericUpDownControl != null) NumbCeillingHeight.NumericUpDownControl.Value = 10;
            if (NumbFloorHeight.NumericUpDownControl != null) NumbFloorHeight.NumericUpDownControl.Value = 0;

            BtnWallColor.BackColor = Color.Black;
            BtnWallColor.ForeColor = Color.Black;
            BtnCeillingColor.BackColor = Color.Black;
            BtnCeillingColor.ForeColor = Color.Black;
            BtnFloorColor.BackColor = Color.Black;
            BtnFloorColor.ForeColor = Color.Black;

            gridEditor.ResetData();
        }

        void BtnFileLoad_Click(object sender, EventArgs e) => OpenFilePanel.ShowDialog();

        void BtnFileSave_Click(object sender, EventArgs e)
        {
            if (fileManager.CurrentOpenedFile != "NULL")
            {
                SaveFile();
                return;
            }

            SaveFilePanel.ShowDialog();
        }

        void BtnFileSaveAs_Click(object sender, EventArgs e) => SaveFilePanel.ShowDialog();

        void BtnEditUndo_Click(object sender, EventArgs e)
        {

        }

        void BtnEditRedo_Click(object sender, EventArgs e)
        {

        }

        void OpenFilePanel_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            using (Stream fileStream = OpenFilePanel.OpenFile())
                LoadFile(fileStream);
        }

        void SaveFilePanel_FileOk(object sender, System.ComponentModel.CancelEventArgs e) => SaveFile();

        void LoadFile(Stream fileStream)
        {
            gridEditor.ResetData();
            fileManager.LoadFromFile(OpenFilePanel.FileName, fileStream, out List<Sector> sectors);
            gridEditor.LoadSectors(sectors);
            Text = "Level Editor - " + Path.GetFileName(SaveFilePanel.FileName);
        }

        void SaveFile()
        {
            bool error = false;
            gridEditor.GetSectors(out List<Sector> sectors);
            using (Stream fileStream = SaveFilePanel.OpenFile())
            {
                error = !fileManager.SaveToFile(SaveFilePanel.FileName, fileStream, ref sectors);
                Text = "Level Editor - " + Path.GetFileName(SaveFilePanel.FileName);
            }

            if (!error || fileManager.CurrentOpenedFile == SaveFilePanel.FileName) return;
            File.Delete(SaveFilePanel.FileName);
        }

        void BtnHelpAbout_Click(object sender, EventArgs e) => about.ShowDialog();

        void MainWindow_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data == null || !e.Data.GetDataPresent(DataFormats.FileDrop)) return;
            string[]? data = (string[]?) e.Data.GetData(DataFormats.FileDrop);

            if (data == null || !File.Exists(data[0])) return;

            using (FileStream stream = File.Open(data[0], FileMode.Open))
                LoadFile(stream);
        }

        void MainWindow_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data == null) return;
            if (e.Data.GetDataPresent(DataFormats.FileDrop)) e.Effect = DragDropEffects.Copy;
            else e.Effect = DragDropEffects.None;
        }

        void MainWindow_Resize(object sender, EventArgs e)
        {
            if (gridEditor == null) return;
            gridEditor.OnResize();
        }

        void BtnWallColor_Click(object sender, EventArgs e)
        {
            using (ColorDialog colorDialog = new ColorDialog())
            {
                colorDialog.FullOpen = true;
                if (colorDialog.ShowDialog() != DialogResult.OK) return;
                BtnWallColor.BackColor = colorDialog.Color;
                BtnWallColor.ForeColor = colorDialog.Color;
                gridEditor.ChangedWallColor(colorDialog.Color);
            }
        }

        void BtnWallTexture_Click(object sender, EventArgs e)
        {

        }

        void NumbCeillingHeight_ValueChanged(object sender, EventArgs e)
        {
            if (NumbCeillingHeight.NumericUpDownControl == null || NumbFloorHeight.NumericUpDownControl == null) return;
            int value = (int) NumbCeillingHeight.NumericUpDownControl.Value;
            int floorValue = (int) NumbFloorHeight.NumericUpDownControl.Value;

            if (value <= floorValue)
            {
                NumbCeillingHeight.NumericUpDownControl.Value += Math.Clamp(floorValue - value, 1, decimal.MaxValue);
                return;
            }

            gridEditor.ChangedSectorCeillingHeight(value);
        }

        void NumbFloorHeight_ValueChanged(object sender, EventArgs e)
        {
            if (NumbCeillingHeight.NumericUpDownControl == null || NumbFloorHeight.NumericUpDownControl == null) return;
            int value = (int) NumbFloorHeight.NumericUpDownControl.Value;
            int ceilValue = (int) NumbCeillingHeight.NumericUpDownControl.Value;

            if (value >= ceilValue)
            {
                NumbFloorHeight.NumericUpDownControl.Value += Math.Clamp(value - ceilValue, decimal.MinValue, -1);
                return;
            }

            gridEditor.ChangedSectorFloorHeight(value);
        }

        void BtnCeillingColor_Click(object sender, EventArgs e)
        {
            using (ColorDialog colorDialog = new ColorDialog())
            {
                colorDialog.FullOpen = true;
                if (colorDialog.ShowDialog() != DialogResult.OK) return;
                BtnCeillingColor.BackColor = colorDialog.Color;
                BtnCeillingColor.ForeColor = colorDialog.Color;
                gridEditor.ChangedSectorCeillingColor(colorDialog.Color);
            }
        }

        void BtnFloorColor_Click(object sender, EventArgs e)
        {
            using (ColorDialog colorDialog = new ColorDialog())
            {
                colorDialog.FullOpen = true;
                if (colorDialog.ShowDialog() != DialogResult.OK) return;
                BtnFloorColor.BackColor = colorDialog.Color;
                BtnFloorColor.ForeColor = colorDialog.Color;
                gridEditor.ChangedSectorFloorColor(colorDialog.Color);
            }
        }

        void BtnCeillingtexture_Click(object sender, EventArgs e)
        {

        }

        void BtnFloorTexture_Click(object sender, EventArgs e)
        {

        }
    }
}
