
namespace LevelEditor
{
    partial class MainWindow
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            Panel panel1;
            ToolStripLabel LblWallColor;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
            ToolStripLabel LblWallTexture;
            ToolStripSeparator toolStripSeparator3;
            ToolStripLabel LblEditModes;
            TableLayoutPanel LayoutPanel;
            Panel panel2;
            ToolStripLabel LblCeillingHeight;
            ToolStripLabel LblFloorHeight;
            ToolStripSeparator toolStripSeparator4;
            ToolStripLabel LblCeillingColor;
            ToolStripLabel LblFloorColor;
            ToolStripSeparator toolStripSeparator5;
            ToolStripLabel LblCeillingTexture;
            ToolStripLabel LblFloorTexture;
            toolStrip1 = new ToolStrip();
            BtnWallColor = new ToolStripButton();
            BtnWallTexture = new ToolStripButton();
            BtnLinesMode = new ToolStripButton();
            BtnEditNodes = new ToolStripButton();
            BtnEditWalls = new ToolStripButton();
            LblEditSectors = new ToolStripButton();
            ImgEditorDraw = new PictureBox();
            toolStrip2 = new ToolStrip();
            NumbCeillingHeight = new ToolStripNumberControl();
            NumbFloorHeight = new ToolStripNumberControl();
            BtnCeillingColor = new ToolStripButton();
            BtnFloorColor = new ToolStripButton();
            BtnCeillingtexture = new ToolStripButton();
            BtnFloorTexture = new ToolStripButton();
            LblSelectionData = new ToolStripLabel();
            MainToolbar = new MenuStrip();
            BtnFile = new ToolStripMenuItem();
            BtnFileNew = new ToolStripMenuItem();
            BtnFileLoad = new ToolStripMenuItem();
            toolStripSeparator2 = new ToolStripSeparator();
            BtnFileSave = new ToolStripMenuItem();
            BtnFileSaveAs = new ToolStripMenuItem();
            BtnEdit = new ToolStripMenuItem();
            BtnEditUndo = new ToolStripMenuItem();
            BtnEditRedo = new ToolStripMenuItem();
            toolStripSeparator1 = new ToolStripSeparator();
            BtnEditSettings = new ToolStripMenuItem();
            BtnHelp = new ToolStripMenuItem();
            BtnHelpAbout = new ToolStripMenuItem();
            BtnCompileMap = new ToolStripMenuItem();
            OpenFilePanel = new OpenFileDialog();
            SaveFilePanel = new SaveFileDialog();
            StatusBar = new StatusStrip();
            LblGridSize = new ToolStripStatusLabel();
            LblOrigin = new ToolStripStatusLabel();
            LblCursor = new ToolStripStatusLabel();
            ColorPanel = new ColorDialog();
            panel1 = new Panel();
            LblWallColor = new ToolStripLabel();
            LblWallTexture = new ToolStripLabel();
            toolStripSeparator3 = new ToolStripSeparator();
            LblEditModes = new ToolStripLabel();
            LayoutPanel = new TableLayoutPanel();
            panel2 = new Panel();
            LblCeillingHeight = new ToolStripLabel();
            LblFloorHeight = new ToolStripLabel();
            toolStripSeparator4 = new ToolStripSeparator();
            LblCeillingColor = new ToolStripLabel();
            LblFloorColor = new ToolStripLabel();
            toolStripSeparator5 = new ToolStripSeparator();
            LblCeillingTexture = new ToolStripLabel();
            LblFloorTexture = new ToolStripLabel();
            panel1.SuspendLayout();
            toolStrip1.SuspendLayout();
            LayoutPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize) ImgEditorDraw).BeginInit();
            panel2.SuspendLayout();
            toolStrip2.SuspendLayout();
            MainToolbar.SuspendLayout();
            StatusBar.SuspendLayout();
            SuspendLayout();
            // 
            // panel1
            // 
            panel1.BorderStyle = BorderStyle.FixedSingle;
            panel1.Controls.Add(toolStrip1);
            panel1.Dock = DockStyle.Fill;
            panel1.Location = new Point(1069, 10);
            panel1.Margin = new Padding(10, 0, 0, 0);
            panel1.Name = "panel1";
            LayoutPanel.SetRowSpan(panel1, 2);
            panel1.Size = new Size(195, 622);
            panel1.TabIndex = 7;
            // 
            // toolStrip1
            // 
            toolStrip1.BackColor = SystemColors.Control;
            toolStrip1.CanOverflow = false;
            toolStrip1.Dock = DockStyle.Fill;
            toolStrip1.GripStyle = ToolStripGripStyle.Hidden;
            toolStrip1.Items.AddRange(new ToolStripItem[] { LblWallColor, BtnWallColor, LblWallTexture, BtnWallTexture, BtnLinesMode, toolStripSeparator3, LblEditModes, BtnEditNodes, BtnEditWalls, LblEditSectors });
            toolStrip1.LayoutStyle = ToolStripLayoutStyle.VerticalStackWithOverflow;
            toolStrip1.Location = new Point(0, 0);
            toolStrip1.Name = "toolStrip1";
            toolStrip1.RenderMode = ToolStripRenderMode.System;
            toolStrip1.Size = new Size(193, 620);
            toolStrip1.Stretch = true;
            toolStrip1.TabIndex = 0;
            toolStrip1.Text = "toolStrip1";
            // 
            // LblWallColor
            // 
            LblWallColor.Name = "LblWallColor";
            LblWallColor.Size = new Size(191, 15);
            LblWallColor.Text = "Wall Color";
            // 
            // BtnWallColor
            // 
            BtnWallColor.AutoToolTip = false;
            BtnWallColor.BackColor = Color.Black;
            BtnWallColor.DisplayStyle = ToolStripItemDisplayStyle.Text;
            BtnWallColor.Image = (Image) resources.GetObject("BtnWallColor.Image");
            BtnWallColor.ImageTransparentColor = Color.Magenta;
            BtnWallColor.Name = "BtnWallColor";
            BtnWallColor.Size = new Size(191, 19);
            BtnWallColor.Text = ".";
            BtnWallColor.Click += BtnWallColor_Click;
            // 
            // LblWallTexture
            // 
            LblWallTexture.Name = "LblWallTexture";
            LblWallTexture.Size = new Size(191, 15);
            LblWallTexture.Text = "Wall Texture";
            // 
            // BtnWallTexture
            // 
            BtnWallTexture.AutoToolTip = false;
            BtnWallTexture.BackColor = Color.Black;
            BtnWallTexture.DisplayStyle = ToolStripItemDisplayStyle.Text;
            BtnWallTexture.Image = (Image) resources.GetObject("BtnWallTexture.Image");
            BtnWallTexture.ImageTransparentColor = Color.Magenta;
            BtnWallTexture.Name = "BtnWallTexture";
            BtnWallTexture.Size = new Size(191, 19);
            BtnWallTexture.Text = ".";
            BtnWallTexture.Click += BtnWallTexture_Click;
            // 
            // BtnLinesMode
            // 
            BtnLinesMode.Image = (Image) resources.GetObject("BtnLinesMode.Image");
            BtnLinesMode.ImageTransparentColor = Color.Magenta;
            BtnLinesMode.Margin = new Padding(10, 5, 5, 0);
            BtnLinesMode.Name = "BtnLinesMode";
            BtnLinesMode.Size = new Size(176, 20);
            BtnLinesMode.Text = "Draw Walls";
            BtnLinesMode.Click += BtnLinesMode_Click;
            // 
            // toolStripSeparator3
            // 
            toolStripSeparator3.MergeAction = MergeAction.Insert;
            toolStripSeparator3.MergeIndex = 0;
            toolStripSeparator3.Name = "toolStripSeparator3";
            toolStripSeparator3.Size = new Size(191, 6);
            // 
            // LblEditModes
            // 
            LblEditModes.MergeAction = MergeAction.Insert;
            LblEditModes.MergeIndex = 0;
            LblEditModes.Name = "LblEditModes";
            LblEditModes.Size = new Size(191, 15);
            LblEditModes.Text = "Edit Modes";
            // 
            // BtnEditNodes
            // 
            BtnEditNodes.Image = (Image) resources.GetObject("BtnEditNodes.Image");
            BtnEditNodes.ImageTransparentColor = Color.Magenta;
            BtnEditNodes.Margin = new Padding(10, 3, 5, 2);
            BtnEditNodes.Name = "BtnEditNodes";
            BtnEditNodes.Size = new Size(176, 20);
            BtnEditNodes.Text = "Nodes";
            BtnEditNodes.Click += BtnEditNodes_Click;
            // 
            // BtnEditWalls
            // 
            BtnEditWalls.Image = (Image) resources.GetObject("BtnEditWalls.Image");
            BtnEditWalls.ImageTransparentColor = Color.Magenta;
            BtnEditWalls.Margin = new Padding(10, 1, 5, 2);
            BtnEditWalls.Name = "BtnEditWalls";
            BtnEditWalls.Size = new Size(176, 20);
            BtnEditWalls.Text = "Walls";
            BtnEditWalls.Click += BtnEditWalls_Click;
            // 
            // LblEditSectors
            // 
            LblEditSectors.Image = (Image) resources.GetObject("LblEditSectors.Image");
            LblEditSectors.ImageTransparentColor = Color.Magenta;
            LblEditSectors.Margin = new Padding(10, 1, 5, 2);
            LblEditSectors.Name = "LblEditSectors";
            LblEditSectors.Size = new Size(176, 20);
            LblEditSectors.Text = "Sectors";
            LblEditSectors.Click += LblEditSectors_Click;
            // 
            // LayoutPanel
            // 
            LayoutPanel.ColumnCount = 2;
            LayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 83.84728F));
            LayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 16.15272F));
            LayoutPanel.Controls.Add(ImgEditorDraw, 0, 0);
            LayoutPanel.Controls.Add(panel1, 1, 0);
            LayoutPanel.Controls.Add(panel2, 0, 1);
            LayoutPanel.Dock = DockStyle.Fill;
            LayoutPanel.Location = new Point(0, 24);
            LayoutPanel.Name = "LayoutPanel";
            LayoutPanel.Padding = new Padding(0, 10, 0, 3);
            LayoutPanel.RowCount = 2;
            LayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
            LayoutPanel.RowStyles.Add(new RowStyle(SizeType.Absolute, 60F));
            LayoutPanel.Size = new Size(1264, 635);
            LayoutPanel.TabIndex = 8;
            // 
            // ImgEditorDraw
            // 
            ImgEditorDraw.BackColor = Color.FromArgb(  35,   35,   35);
            ImgEditorDraw.BorderStyle = BorderStyle.FixedSingle;
            ImgEditorDraw.Dock = DockStyle.Fill;
            ImgEditorDraw.Location = new Point(0, 10);
            ImgEditorDraw.Margin = new Padding(0);
            ImgEditorDraw.Name = "ImgEditorDraw";
            ImgEditorDraw.Size = new Size(1059, 562);
            ImgEditorDraw.TabIndex = 0;
            ImgEditorDraw.TabStop = false;
            ImgEditorDraw.Paint += ImgEditorDraw_Paint;
            ImgEditorDraw.MouseDown += ImgEditorDraw_MouseDown;
            ImgEditorDraw.MouseEnter += ImgEditorDraw_MouseEnter;
            ImgEditorDraw.MouseLeave += ImgEditorDraw_MouseLeave;
            ImgEditorDraw.MouseMove += ImgEditorDraw_MouseMove;
            ImgEditorDraw.MouseUp += ImgEditorDraw_MouseUp;
            // 
            // panel2
            // 
            panel2.BorderStyle = BorderStyle.FixedSingle;
            panel2.Controls.Add(toolStrip2);
            panel2.Dock = DockStyle.Fill;
            panel2.Location = new Point(0, 575);
            panel2.Margin = new Padding(0, 3, 0, 0);
            panel2.Name = "panel2";
            panel2.Size = new Size(1059, 57);
            panel2.TabIndex = 8;
            // 
            // toolStrip2
            // 
            toolStrip2.BackColor = SystemColors.Control;
            toolStrip2.Dock = DockStyle.Fill;
            toolStrip2.GripStyle = ToolStripGripStyle.Hidden;
            toolStrip2.Items.AddRange(new ToolStripItem[] { LblCeillingHeight, NumbCeillingHeight, LblFloorHeight, NumbFloorHeight, toolStripSeparator4, LblCeillingColor, BtnCeillingColor, LblFloorColor, BtnFloorColor, toolStripSeparator5, LblCeillingTexture, BtnCeillingtexture, LblFloorTexture, BtnFloorTexture, LblSelectionData });
            toolStrip2.Location = new Point(0, 0);
            toolStrip2.Name = "toolStrip2";
            toolStrip2.RenderMode = ToolStripRenderMode.System;
            toolStrip2.Size = new Size(1057, 55);
            toolStrip2.TabIndex = 0;
            toolStrip2.Text = "toolStrip2";
            // 
            // LblCeillingHeight
            // 
            LblCeillingHeight.BackColor = SystemColors.Control;
            LblCeillingHeight.Name = "LblCeillingHeight";
            LblCeillingHeight.Size = new Size(86, 52);
            LblCeillingHeight.Text = "Ceilling Height";
            // 
            // NumbCeillingHeight
            // 
            NumbCeillingHeight.Name = "NumbCeillingHeight";
            NumbCeillingHeight.RightToLeft = RightToLeft.No;
            NumbCeillingHeight.Size = new Size(41, 52);
            NumbCeillingHeight.Text = "10";
            NumbCeillingHeight.ValueChanged += NumbCeillingHeight_ValueChanged;
            // 
            // LblFloorHeight
            // 
            LblFloorHeight.BackColor = SystemColors.Control;
            LblFloorHeight.Name = "LblFloorHeight";
            LblFloorHeight.Size = new Size(73, 52);
            LblFloorHeight.Text = "Floor Height";
            // 
            // NumbFloorHeight
            // 
            NumbFloorHeight.Name = "NumbFloorHeight";
            NumbFloorHeight.Size = new Size(41, 52);
            NumbFloorHeight.Text = "0";
            NumbFloorHeight.ValueChanged += NumbFloorHeight_ValueChanged;
            // 
            // toolStripSeparator4
            // 
            toolStripSeparator4.Name = "toolStripSeparator4";
            toolStripSeparator4.Size = new Size(6, 55);
            // 
            // LblCeillingColor
            // 
            LblCeillingColor.BackColor = SystemColors.Control;
            LblCeillingColor.Name = "LblCeillingColor";
            LblCeillingColor.Size = new Size(79, 52);
            LblCeillingColor.Text = "Ceilling Color";
            // 
            // BtnCeillingColor
            // 
            BtnCeillingColor.AutoToolTip = false;
            BtnCeillingColor.BackColor = Color.Black;
            BtnCeillingColor.DisplayStyle = ToolStripItemDisplayStyle.Text;
            BtnCeillingColor.Image = (Image) resources.GetObject("BtnCeillingColor.Image");
            BtnCeillingColor.ImageTransparentColor = Color.Magenta;
            BtnCeillingColor.Name = "BtnCeillingColor";
            BtnCeillingColor.Size = new Size(80, 52);
            BtnCeillingColor.Text = ".......................";
            BtnCeillingColor.Click += BtnCeillingColor_Click;
            // 
            // LblFloorColor
            // 
            LblFloorColor.BackColor = SystemColors.Control;
            LblFloorColor.Name = "LblFloorColor";
            LblFloorColor.Size = new Size(66, 52);
            LblFloorColor.Text = "Floor Color";
            // 
            // BtnFloorColor
            // 
            BtnFloorColor.AutoToolTip = false;
            BtnFloorColor.BackColor = Color.Black;
            BtnFloorColor.DisplayStyle = ToolStripItemDisplayStyle.Text;
            BtnFloorColor.Image = (Image) resources.GetObject("BtnFloorColor.Image");
            BtnFloorColor.ImageTransparentColor = Color.Magenta;
            BtnFloorColor.Name = "BtnFloorColor";
            BtnFloorColor.Size = new Size(80, 52);
            BtnFloorColor.Text = ".......................";
            BtnFloorColor.Click += BtnFloorColor_Click;
            // 
            // toolStripSeparator5
            // 
            toolStripSeparator5.Name = "toolStripSeparator5";
            toolStripSeparator5.Size = new Size(6, 55);
            // 
            // LblCeillingTexture
            // 
            LblCeillingTexture.BackColor = SystemColors.Control;
            LblCeillingTexture.Name = "LblCeillingTexture";
            LblCeillingTexture.Size = new Size(88, 52);
            LblCeillingTexture.Text = "Ceilling Texture";
            // 
            // BtnCeillingtexture
            // 
            BtnCeillingtexture.AutoToolTip = false;
            BtnCeillingtexture.BackColor = Color.Black;
            BtnCeillingtexture.DisplayStyle = ToolStripItemDisplayStyle.Text;
            BtnCeillingtexture.Image = (Image) resources.GetObject("BtnCeillingtexture.Image");
            BtnCeillingtexture.ImageTransparentColor = Color.Magenta;
            BtnCeillingtexture.Name = "BtnCeillingtexture";
            BtnCeillingtexture.Size = new Size(80, 52);
            BtnCeillingtexture.Text = ".......................";
            BtnCeillingtexture.Click += BtnCeillingtexture_Click;
            // 
            // LblFloorTexture
            // 
            LblFloorTexture.BackColor = SystemColors.Control;
            LblFloorTexture.Name = "LblFloorTexture";
            LblFloorTexture.Size = new Size(75, 52);
            LblFloorTexture.Text = "Floor Texture";
            // 
            // BtnFloorTexture
            // 
            BtnFloorTexture.AutoToolTip = false;
            BtnFloorTexture.BackColor = Color.Black;
            BtnFloorTexture.DisplayStyle = ToolStripItemDisplayStyle.Text;
            BtnFloorTexture.Image = (Image) resources.GetObject("BtnFloorTexture.Image");
            BtnFloorTexture.ImageTransparentColor = Color.Magenta;
            BtnFloorTexture.Name = "BtnFloorTexture";
            BtnFloorTexture.Size = new Size(80, 52);
            BtnFloorTexture.Text = ".......................";
            BtnFloorTexture.Click += BtnFloorTexture_Click;
            // 
            // LblSelectionData
            // 
            LblSelectionData.Enabled = false;
            LblSelectionData.Name = "LblSelectionData";
            LblSelectionData.Size = new Size(82, 52);
            LblSelectionData.Text = "Selection Data";
            // 
            // MainToolbar
            // 
            MainToolbar.BackColor = Color.White;
            MainToolbar.Items.AddRange(new ToolStripItem[] { BtnFile, BtnEdit, BtnHelp, BtnCompileMap });
            MainToolbar.Location = new Point(0, 0);
            MainToolbar.Name = "MainToolbar";
            MainToolbar.RenderMode = ToolStripRenderMode.System;
            MainToolbar.Size = new Size(1264, 24);
            MainToolbar.TabIndex = 5;
            MainToolbar.Text = "ToolBar";
            // 
            // BtnFile
            // 
            BtnFile.BackColor = SystemColors.Control;
            BtnFile.DropDownItems.AddRange(new ToolStripItem[] { BtnFileNew, BtnFileLoad, toolStripSeparator2, BtnFileSave, BtnFileSaveAs });
            BtnFile.ForeColor = SystemColors.ControlText;
            BtnFile.Name = "BtnFile";
            BtnFile.Size = new Size(37, 20);
            BtnFile.Text = "File";
            // 
            // BtnFileNew
            // 
            BtnFileNew.Name = "BtnFileNew";
            BtnFileNew.Size = new Size(114, 22);
            BtnFileNew.Text = "New";
            BtnFileNew.Click += BtnFileNew_Click;
            // 
            // BtnFileLoad
            // 
            BtnFileLoad.Name = "BtnFileLoad";
            BtnFileLoad.Size = new Size(114, 22);
            BtnFileLoad.Text = "Load";
            BtnFileLoad.Click += BtnFileLoad_Click;
            // 
            // toolStripSeparator2
            // 
            toolStripSeparator2.Name = "toolStripSeparator2";
            toolStripSeparator2.Size = new Size(111, 6);
            // 
            // BtnFileSave
            // 
            BtnFileSave.Name = "BtnFileSave";
            BtnFileSave.Size = new Size(114, 22);
            BtnFileSave.Text = "Save";
            BtnFileSave.Click += BtnFileSave_Click;
            // 
            // BtnFileSaveAs
            // 
            BtnFileSaveAs.Name = "BtnFileSaveAs";
            BtnFileSaveAs.Size = new Size(114, 22);
            BtnFileSaveAs.Text = "Save &As";
            BtnFileSaveAs.Click += BtnFileSaveAs_Click;
            // 
            // BtnEdit
            // 
            BtnEdit.DropDownItems.AddRange(new ToolStripItem[] { BtnEditUndo, BtnEditRedo, toolStripSeparator1, BtnEditSettings });
            BtnEdit.ForeColor = SystemColors.ControlText;
            BtnEdit.Name = "BtnEdit";
            BtnEdit.Size = new Size(39, 20);
            BtnEdit.Text = "Edit";
            // 
            // BtnEditUndo
            // 
            BtnEditUndo.Name = "BtnEditUndo";
            BtnEditUndo.Size = new Size(116, 22);
            BtnEditUndo.Text = "Undo";
            BtnEditUndo.Click += BtnEditUndo_Click;
            // 
            // BtnEditRedo
            // 
            BtnEditRedo.Name = "BtnEditRedo";
            BtnEditRedo.Size = new Size(116, 22);
            BtnEditRedo.Text = "Redo";
            BtnEditRedo.Click += BtnEditRedo_Click;
            // 
            // toolStripSeparator1
            // 
            toolStripSeparator1.Name = "toolStripSeparator1";
            toolStripSeparator1.Size = new Size(113, 6);
            // 
            // BtnEditSettings
            // 
            BtnEditSettings.Name = "BtnEditSettings";
            BtnEditSettings.Size = new Size(116, 22);
            BtnEditSettings.Text = "Settings";
            // 
            // BtnHelp
            // 
            BtnHelp.BackColor = SystemColors.Window;
            BtnHelp.DropDownItems.AddRange(new ToolStripItem[] { BtnHelpAbout });
            BtnHelp.ForeColor = SystemColors.ControlText;
            BtnHelp.Name = "BtnHelp";
            BtnHelp.Size = new Size(44, 20);
            BtnHelp.Text = "Help";
            // 
            // BtnHelpAbout
            // 
            BtnHelpAbout.Name = "BtnHelpAbout";
            BtnHelpAbout.Size = new Size(107, 22);
            BtnHelpAbout.Text = "About";
            BtnHelpAbout.Click += BtnHelpAbout_Click;
            // 
            // BtnCompileMap
            // 
            BtnCompileMap.Name = "BtnCompileMap";
            BtnCompileMap.Size = new Size(64, 20);
            BtnCompileMap.Text = "Compile";
            BtnCompileMap.Click += BtnCompileMap_Click;
            // 
            // OpenFilePanel
            // 
            OpenFilePanel.DefaultExt = "map";
            OpenFilePanel.Filter = ".map Files|*.map";
            OpenFilePanel.FileOk += OpenFilePanel_FileOk;
            // 
            // SaveFilePanel
            // 
            SaveFilePanel.DefaultExt = "map";
            SaveFilePanel.Filter = ".map Files|*.map";
            SaveFilePanel.FileOk += SaveFilePanel_FileOk;
            // 
            // StatusBar
            // 
            StatusBar.BackColor = SystemColors.Desktop;
            StatusBar.Items.AddRange(new ToolStripItem[] { LblGridSize, LblOrigin, LblCursor });
            StatusBar.Location = new Point(0, 659);
            StatusBar.Name = "StatusBar";
            StatusBar.RightToLeft = RightToLeft.Yes;
            StatusBar.Size = new Size(1264, 22);
            StatusBar.SizingGrip = false;
            StatusBar.TabIndex = 6;
            StatusBar.Text = "Statusbar";
            // 
            // LblGridSize
            // 
            LblGridSize.ForeColor = Color.White;
            LblGridSize.Name = "LblGridSize";
            LblGridSize.RightToLeft = RightToLeft.No;
            LblGridSize.Size = new Size(79, 17);
            LblGridSize.Text = "GridSize: 0000";
            // 
            // LblOrigin
            // 
            LblOrigin.ForeColor = Color.White;
            LblOrigin.Name = "LblOrigin";
            LblOrigin.RightToLeft = RightToLeft.No;
            LblOrigin.Size = new Size(108, 17);
            LblOrigin.Text = "Origin: (0000, 0000)";
            // 
            // LblCursor
            // 
            LblCursor.ForeColor = Color.White;
            LblCursor.Name = "LblCursor";
            LblCursor.RightToLeft = RightToLeft.No;
            LblCursor.Size = new Size(110, 17);
            LblCursor.Text = "Cursor: (0000, 0000)";
            // 
            // ColorPanel
            // 
            ColorPanel.AnyColor = true;
            ColorPanel.FullOpen = true;
            // 
            // MainWindow
            // 
            AllowDrop = true;
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = Color.Black;
            ClientSize = new Size(1264, 681);
            Controls.Add(LayoutPanel);
            Controls.Add(StatusBar);
            Controls.Add(MainToolbar);
            ForeColor = Color.Black;
            KeyPreview = true;
            MainMenuStrip = MainToolbar;
            Name = "MainWindow";
            Text = "Level Editor";
            DragDrop += MainWindow_DragDrop;
            DragEnter += MainWindow_DragEnter;
            Resize += MainWindow_Resize;
            panel1.ResumeLayout(false);
            panel1.PerformLayout();
            toolStrip1.ResumeLayout(false);
            toolStrip1.PerformLayout();
            LayoutPanel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize) ImgEditorDraw).EndInit();
            panel2.ResumeLayout(false);
            panel2.PerformLayout();
            toolStrip2.ResumeLayout(false);
            toolStrip2.PerformLayout();
            MainToolbar.ResumeLayout(false);
            MainToolbar.PerformLayout();
            StatusBar.ResumeLayout(false);
            StatusBar.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private MenuStrip MainToolbar;
        private ToolStripMenuItem BtnFile;
        private ToolStripMenuItem BtnFileNew;
        private ToolStripMenuItem BtnFileLoad;
        private ToolStripMenuItem BtnEdit;
        private ToolStripMenuItem BtnEditUndo;
        private ToolStripMenuItem BtnEditRedo;
        private ToolStripMenuItem BtnFileSave;
        private OpenFileDialog OpenFilePanel;
        private SaveFileDialog SaveFilePanel;
        private ToolStripSeparator toolStripSeparator1;
        private ToolStripMenuItem BtnEditSettings;
        private ToolStripMenuItem BtnHelp;
        private ToolStripMenuItem BtnHelpAbout;
        private ToolStripMenuItem BtnFileSaveAs;
        private ToolStripSeparator toolStripSeparator2;
        private StatusStrip StatusBar;
        private ToolStripStatusLabel LblGridSize;
        private ToolStripStatusLabel LblOrigin;
        private ToolStripStatusLabel LblCursor;
        private Panel panel1;
        private ToolStrip toolStrip1;
        private TableLayoutPanel LayoutPanel;
        private PictureBox ImgEditorDraw;
        private ToolStripButton BtnLinesMode;
        private ToolStripSeparator toolStripSeparator3;
        private ToolStripLabel LblEditModes;
        private ToolStripButton BtnEditNodes;
        private ToolStripButton BtnEditWalls;
        private ToolStripButton LblEditSectors;
        private Panel panel2;
        private ToolStrip toolStrip2;
        private ToolStripLabel LblCeillingHeight;
        private ToolStripLabel LblFloorHeight;
        private ToolStripLabel LblFloorColor;
        private ToolStripLabel LblCeillingColor;
        private ToolStripLabel LblWallTexture;
        private ToolStripLabel LblFloorTexture;
        private ToolStripLabel LblCeillingTexture;
        private ToolStripLabel LblWallColor;
        private ToolStripButton BtnWallTexture;
        private ToolStripButton BtnWallColor;
        private ToolStripButton BtnCeillingColor;
        private ToolStripButton BtnFloorColor;
        private ToolStripButton BtnCeillingtexture;
        private ToolStripButton BtnFloorTexture;
        private ToolStripNumberControl NumbFloorHeight;
        private ToolStripNumberControl NumbCeillingHeight;
        private ColorDialog ColorPanel;
        private ToolStripMenuItem BtnCompileMap;
        private ToolStripLabel LblSelectionData;
    }
}
