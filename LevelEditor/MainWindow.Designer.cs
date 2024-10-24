
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
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
            OpenFilePanel = new OpenFileDialog();
            SaveFilePanel = new SaveFileDialog();
            StatusBar = new StatusStrip();
            LblGridSize = new ToolStripStatusLabel();
            LblOrigin = new ToolStripStatusLabel();
            LblCursor = new ToolStripStatusLabel();
            panel1 = new Panel();
            toolStrip1 = new ToolStrip();
            BtnLinesMode = new ToolStripButton();
            ImgEditorDraw = new PictureBox();
            LayoutPanel = new TableLayoutPanel();
            label1 = new Label();
            MainToolbar.SuspendLayout();
            StatusBar.SuspendLayout();
            panel1.SuspendLayout();
            toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize) ImgEditorDraw).BeginInit();
            LayoutPanel.SuspendLayout();
            SuspendLayout();
            // 
            // MainToolbar
            // 
            MainToolbar.BackColor = Color.White;
            MainToolbar.Items.AddRange(new ToolStripItem[] { BtnFile, BtnEdit, BtnHelp });
            MainToolbar.Location = new Point(0, 0);
            MainToolbar.Name = "MainToolbar";
            MainToolbar.RenderMode = ToolStripRenderMode.System;
            MainToolbar.Size = new Size(804, 24);
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
            StatusBar.Location = new Point(0, 429);
            StatusBar.Name = "StatusBar";
            StatusBar.RightToLeft = RightToLeft.Yes;
            StatusBar.Size = new Size(804, 22);
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
            // panel1
            // 
            panel1.BorderStyle = BorderStyle.FixedSingle;
            panel1.Controls.Add(label1);
            panel1.Controls.Add(toolStrip1);
            panel1.Dock = DockStyle.Fill;
            panel1.Location = new Point(684, 10);
            panel1.Margin = new Padding(10, 0, 0, 0);
            panel1.Name = "panel1";
            panel1.Size = new Size(120, 385);
            panel1.TabIndex = 7;
            // 
            // toolStrip1
            // 
            toolStrip1.BackColor = Color.White;
            toolStrip1.GripStyle = ToolStripGripStyle.Hidden;
            toolStrip1.Items.AddRange(new ToolStripItem[] { BtnLinesMode });
            toolStrip1.Location = new Point(0, 0);
            toolStrip1.Name = "toolStrip1";
            toolStrip1.RenderMode = ToolStripRenderMode.System;
            toolStrip1.Size = new Size(118, 25);
            toolStrip1.TabIndex = 0;
            toolStrip1.Text = "toolStrip1";
            // 
            // BtnLinesMode
            // 
            BtnLinesMode.Image = (Image) resources.GetObject("BtnLinesMode.Image");
            BtnLinesMode.ImageTransparentColor = Color.Magenta;
            BtnLinesMode.Margin = new Padding(10, 1, 0, 2);
            BtnLinesMode.Name = "BtnLinesMode";
            BtnLinesMode.Size = new Size(84, 22);
            BtnLinesMode.Text = "Draw Lines";
            BtnLinesMode.Click += BtnLinesMode_Click;
            // 
            // ImgEditorDraw
            // 
            ImgEditorDraw.BackColor = Color.FromArgb(  35,   35,   35);
            ImgEditorDraw.BorderStyle = BorderStyle.FixedSingle;
            ImgEditorDraw.Dock = DockStyle.Fill;
            ImgEditorDraw.Location = new Point(0, 10);
            ImgEditorDraw.Margin = new Padding(0, 0, 5, 0);
            ImgEditorDraw.Name = "ImgEditorDraw";
            ImgEditorDraw.Size = new Size(669, 385);
            ImgEditorDraw.TabIndex = 0;
            ImgEditorDraw.TabStop = false;
            ImgEditorDraw.Paint += ImgEditorDraw_Paint;
            ImgEditorDraw.MouseDown += ImgEditorDraw_MouseDown;
            ImgEditorDraw.MouseEnter += ImgEditorDraw_MouseEnter;
            ImgEditorDraw.MouseLeave += ImgEditorDraw_MouseLeave;
            ImgEditorDraw.MouseMove += ImgEditorDraw_MouseMove;
            ImgEditorDraw.MouseUp += ImgEditorDraw_MouseUp;
            // 
            // LayoutPanel
            // 
            LayoutPanel.ColumnCount = 2;
            LayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 83.84728F));
            LayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 16.1527157F));
            LayoutPanel.Controls.Add(ImgEditorDraw, 0, 0);
            LayoutPanel.Controls.Add(panel1, 1, 0);
            LayoutPanel.Dock = DockStyle.Fill;
            LayoutPanel.Location = new Point(0, 24);
            LayoutPanel.Name = "LayoutPanel";
            LayoutPanel.Padding = new Padding(0, 10, 0, 10);
            LayoutPanel.RowCount = 1;
            LayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 75.80247F));
            LayoutPanel.Size = new Size(804, 405);
            LayoutPanel.TabIndex = 8;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(20, 154);
            label1.Name = "label1";
            label1.Size = new Size(38, 15);
            label1.TabIndex = 1;
            label1.Text = "label1";
            // 
            // MainWindow
            // 
            AllowDrop = true;
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = Color.Black;
            ClientSize = new Size(804, 451);
            Controls.Add(LayoutPanel);
            Controls.Add(StatusBar);
            Controls.Add(MainToolbar);
            MainMenuStrip = MainToolbar;
            Name = "MainWindow";
            Text = "Level Editor";
            DragDrop += MainWindow_DragDrop;
            DragEnter += MainWindow_DragEnter;
            Resize += MainWindow_Resize;
            MainToolbar.ResumeLayout(false);
            MainToolbar.PerformLayout();
            StatusBar.ResumeLayout(false);
            StatusBar.PerformLayout();
            panel1.ResumeLayout(false);
            panel1.PerformLayout();
            toolStrip1.ResumeLayout(false);
            toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize) ImgEditorDraw).EndInit();
            LayoutPanel.ResumeLayout(false);
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
        private ToolStripButton BtnLinesMode;
        private TableLayoutPanel LayoutPanel;
        private PictureBox ImgEditorDraw;
        private Label label1;
    }
}
