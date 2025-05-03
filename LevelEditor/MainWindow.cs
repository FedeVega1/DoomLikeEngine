namespace LevelEditor
{
    public partial class MainWindow : Form
    {
        MapGridEditor gridEditor;
        FileManager fileManager;
        AboutWindow about;
        BSPAnalyzer analyzer;
        System.Windows.Forms.Timer shiftCtrlTimer;

        public MainWindow()
        {
            InitializeComponent();
            Text = "Level Editor - NewProject.map";
            SaveFilePanel.FileName = "NewProject";

            WindowState = FormWindowState.Maximized;
            shiftCtrlTimer = new System.Windows.Forms.Timer();
            shiftCtrlTimer.Tick += CheckShiftCtrlKey;
            shiftCtrlTimer.Start();

            FormClosed += OnFormClosed;

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
            analyzer = new BSPAnalyzer();
            COLoggerImport.InitLogSys(true, false);

            gridEditor = new MapGridEditor(new GridEditorData(ref LblCursor, ref LblOrigin, ref LblGridSize,
                ref ImgEditorDraw, ref BtnWallTopColor, ref BtnWallInColor, ref BtnWallBtmColor, ref BtnCeillingColor,
                ref BtnFloorColor, ref NumbCeillingHeight, ref NumbFloorHeight, ref LblSelectionData));
            fileManager = new FileManager();
        }

        void OnFormClosed(object sender, FormClosedEventArgs e)
        {
            shiftCtrlTimer.Tick -= CheckShiftCtrlKey;
            shiftCtrlTimer.Stop();
            shiftCtrlTimer.Dispose();
        }

        void CheckShiftCtrlKey(object sender, EventArgs e) => gridEditor.ShiftKeyDown = gridEditor.CtrlKeyDown = false;

        protected override bool ProcessKeyPreview(ref Message m)
        {
            COLoggerImport.LogNormal((Keys)m.WParam);
            switch ((Keys)m.WParam)
            {
                case Keys.ShiftKey:
                    gridEditor.ShiftKeyDown = m.Msg == 0x0100;
                    return true;

                case Keys.ControlKey:
                    gridEditor.ShiftKeyDown = m.Msg == 0x0100;
                    return true;
            }

            return base.ProcessKeyPreview(ref m);
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (NumbCeillingHeight.Focused || NumbFloorHeight.Focused)
                return base.ProcessCmdKey(ref msg, keyData);

            Keys processedKeyData = keyData;
            if ((processedKeyData & Keys.Shift) == Keys.Shift) processedKeyData ^= Keys.Shift;
            if ((processedKeyData & Keys.Control) == Keys.Control) processedKeyData ^= Keys.Control;

            switch (processedKeyData)
            {
                case Keys.Add:
                    gridEditor.UpdateGridSize(1);
                    return true;

                case Keys.Subtract:
                    gridEditor.UpdateGridSize(-1);
                    return true;

                case Keys.Escape:
                    gridEditor.ExitCurrentMode();
                    return true;

                case Keys.Up: gridEditor.MoveOrigin(new PointF(0, 1)); return true;
                case Keys.Down: gridEditor.MoveOrigin(new PointF(0, -1)); return true;
                case Keys.Left: gridEditor.MoveOrigin(new PointF(1, 0)); return true;
                case Keys.Right: gridEditor.MoveOrigin(new PointF(-1, 0)); return true;

                case Keys.ShiftKey:
                    gridEditor.ShiftKeyDown = msg.Msg == 0x0100;
                    return true;

                case Keys.ControlKey:
                    gridEditor.CtrlKeyDown = msg.Msg == 0x0100;
                    return true;

                case Keys.Delete:
                    gridEditor.DeleteCurrentSelection();
                    break;
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

        void BtnLinesMode_Click(object sender, EventArgs e) => gridEditor.ChangeCurrentMode(EditorMode.LineMode);
        void BtnEditNodes_Click(object sender, EventArgs e) => gridEditor.ChangeCurrentMode(EditorMode.NodeMode);
        void BtnEditWalls_Click(object sender, EventArgs e) => gridEditor.ChangeCurrentMode(EditorMode.WallMode);
        void LblEditSectors_Click(object sender, EventArgs e) => gridEditor.ChangeCurrentMode(EditorMode.SectorMode);

        void BtnFileNew_Click(object sender, EventArgs e)
        {
            fileManager.ResetCurrentFile();
            Text = "Level Editor - NewProject.map";

            if (NumbCeillingHeight.NumericUpDownControl != null) NumbCeillingHeight.NumericUpDownControl.Value = 10;
            if (NumbFloorHeight.NumericUpDownControl != null) NumbFloorHeight.NumericUpDownControl.Value = 0;

            BtnWallBtmColor.BackColor = Color.Black;
            BtnWallBtmColor.ForeColor = Color.Black;
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
                using (Stream fileStream = File.OpenWrite(OpenFilePanel.FileName))
                    SaveFile(OpenFilePanel.FileName, fileStream);
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

        void SaveFilePanel_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            using (Stream fileStream = SaveFilePanel.OpenFile())
                SaveFile(SaveFilePanel.FileName, fileStream);
        }

        void LoadFile(Stream fileStream)
        {
            gridEditor.ResetData();
            if (!fileManager.LoadFromFile(OpenFilePanel.FileName, fileStream, out List<Sector> sectors)) return;
            gridEditor.LoadSectors(sectors);
            Text = "Level Editor - " + Path.GetFileName(OpenFilePanel.FileName);
        }

        void SaveFile(string fileName, Stream fileStream)
        {
            gridEditor.GetSectors(out List<Sector> sectors);
            Text = "Level Editor - " + Path.GetFileName(fileName);
            if (fileManager.SaveToFile(fileName, fileStream, ref sectors) || fileManager.CurrentOpenedFile == fileName) return;
            fileStream.Close();
            File.Delete(fileName);
        }

        void BtnHelpAbout_Click(object sender, EventArgs e) => about.ShowDialog();

        void MainWindow_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data == null || !e.Data.GetDataPresent(DataFormats.FileDrop)) return;
            string[] data = (string[])e.Data.GetData(DataFormats.FileDrop);

            if (data == null || !File.Exists(data[0]) || Path.GetExtension(data[0]) != ".map") return;

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

        void BtnWallTopColor_Click(object sender, EventArgs e)
        {
            using (ColorDialog colorDialog = new ColorDialog())
            {
                colorDialog.FullOpen = true;
                colorDialog.Color = BtnWallTopColor.BackColor;
                if (colorDialog.ShowDialog() != DialogResult.OK) return;
                BtnWallTopColor.BackColor = colorDialog.Color;
                BtnWallTopColor.ForeColor = colorDialog.Color;
                gridEditor.ChangedWallTopColor(colorDialog.Color);
            }
        }

        void BtnWallInColor_Click(object sender, EventArgs e)
        {
            using (ColorDialog colorDialog = new ColorDialog())
            {
                colorDialog.FullOpen = true;
                colorDialog.Color = BtnWallInColor.BackColor;
                if (colorDialog.ShowDialog() != DialogResult.OK) return;
                BtnWallInColor.BackColor = colorDialog.Color;
                BtnWallInColor.ForeColor = colorDialog.Color;
                gridEditor.ChangedWallInnerColor(colorDialog.Color);
            }
        }

        void BtnWallBtmColor_Click(object sender, EventArgs e)
        {
            using (ColorDialog colorDialog = new ColorDialog())
            {
                colorDialog.FullOpen = true;
                colorDialog.Color = BtnWallBtmColor.BackColor;
                if (colorDialog.ShowDialog() != DialogResult.OK) return;
                BtnWallBtmColor.BackColor = colorDialog.Color;
                BtnWallBtmColor.ForeColor = colorDialog.Color;
                gridEditor.ChangedWallBottomColor(colorDialog.Color);
            }
        }

        void BtnWallTexture_Click(object sender, EventArgs e)
        {

        }

        void NumbCeillingHeight_ValueChanged(object sender, EventArgs e)
        {
            if (NumbCeillingHeight.NumericUpDownControl == null || NumbFloorHeight.NumericUpDownControl == null) return;
            int value = (int)NumbCeillingHeight.NumericUpDownControl.Value;
            int floorValue = (int)NumbFloorHeight.NumericUpDownControl.Value;

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
            int value = (int)NumbFloorHeight.NumericUpDownControl.Value;
            int ceilValue = (int)NumbCeillingHeight.NumericUpDownControl.Value;

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
                colorDialog.Color = BtnCeillingColor.BackColor;
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
                colorDialog.Color = BtnFloorColor.BackColor;
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

        void BtnCompileMap_Click(object sender, EventArgs e)
        {
            if (fileManager.CurrentOpenedFile == "NULL")
            {
                if (SaveFilePanel.ShowDialog() != DialogResult.OK)
                    return;
            }

            string fileName = Path.ChangeExtension(fileManager.CurrentOpenedFile, ".bsp");
            using (Stream fileStream = File.Create(fileName))
            {
                gridEditor.GetSectors(out List<Sector> sectors);

                if (fileManager.CompileMap(fileName, fileStream, ref sectors)) return;
                fileStream.Close();
                File.Delete(fileName);
            }
        }

        void BtnEditAnalyze_Click(object sender, EventArgs e)
        {
            gridEditor.GetSectors(out List<Sector> sectors);
            analyzer.InitAnalyzer(sectors);
            analyzer.ShowDialog();
        }
    }
}
