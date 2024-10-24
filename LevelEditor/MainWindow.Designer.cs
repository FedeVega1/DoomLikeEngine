
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
            EditorPanel = new Panel();
            ImgEditorDraw = new PictureBox();
            BrushPanel = new Panel();
            BtnLines = new Button();
            LblOrigin = new Label();
            LblCursor = new Label();
            LblGridSize = new Label();
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
            EditorPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize) ImgEditorDraw).BeginInit();
            BrushPanel.SuspendLayout();
            MainToolbar.SuspendLayout();
            SuspendLayout();
            // 
            // EditorPanel
            // 
            EditorPanel.BackColor = Color.White;
            EditorPanel.BorderStyle = BorderStyle.FixedSingle;
            EditorPanel.Controls.Add(ImgEditorDraw);
            EditorPanel.Location = new Point(12, 41);
            EditorPanel.Name = "EditorPanel";
            EditorPanel.Size = new Size(663, 378);
            EditorPanel.TabIndex = 1;
            // 
            // ImgEditorDraw
            // 
            ImgEditorDraw.BackColor = Color.FromArgb(  35,   35,   35);
            ImgEditorDraw.Location = new Point(-1, -1);
            ImgEditorDraw.Name = "ImgEditorDraw";
            ImgEditorDraw.Size = new Size(663, 378);
            ImgEditorDraw.TabIndex = 0;
            ImgEditorDraw.TabStop = false;
            ImgEditorDraw.Paint += ImgEditorDraw_Paint;
            ImgEditorDraw.MouseDown += ImgEditorDraw_MouseDown;
            ImgEditorDraw.MouseEnter += ImgEditorDraw_MouseEnter;
            ImgEditorDraw.MouseLeave += ImgEditorDraw_MouseLeave;
            ImgEditorDraw.MouseMove += ImgEditorDraw_MouseMove;
            ImgEditorDraw.MouseUp += ImgEditorDraw_MouseUp;
            // 
            // BrushPanel
            // 
            BrushPanel.BorderStyle = BorderStyle.FixedSingle;
            BrushPanel.Controls.Add(BtnLines);
            BrushPanel.Location = new Point(691, 41);
            BrushPanel.Name = "BrushPanel";
            BrushPanel.Size = new Size(107, 378);
            BrushPanel.TabIndex = 1;
            // 
            // BtnLines
            // 
            BtnLines.Location = new Point(12, 14);
            BtnLines.Name = "BtnLines";
            BtnLines.Size = new Size(75, 23);
            BtnLines.TabIndex = 0;
            BtnLines.Text = "Draw Lines";
            BtnLines.UseVisualStyleBackColor = true;
            BtnLines.Click += BtnLines_Click;
            // 
            // LblOrigin
            // 
            LblOrigin.Anchor =  AnchorStyles.Bottom | AnchorStyles.Right;
            LblOrigin.AutoSize = true;
            LblOrigin.ForeColor = SystemColors.Control;
            LblOrigin.Location = new Point(567, 426);
            LblOrigin.Name = "LblOrigin";
            LblOrigin.Size = new Size(108, 15);
            LblOrigin.TabIndex = 2;
            LblOrigin.Text = "Origin: (0000, 0000)";
            // 
            // LblCursor
            // 
            LblCursor.Anchor =  AnchorStyles.Bottom | AnchorStyles.Right;
            LblCursor.AutoSize = true;
            LblCursor.ForeColor = SystemColors.Control;
            LblCursor.Location = new Point(451, 426);
            LblCursor.Name = "LblCursor";
            LblCursor.Size = new Size(110, 15);
            LblCursor.TabIndex = 3;
            LblCursor.Text = "Cursor: (0000, 0000)";
            // 
            // LblGridSize
            // 
            LblGridSize.Anchor =  AnchorStyles.Bottom | AnchorStyles.Right;
            LblGridSize.AutoSize = true;
            LblGridSize.ForeColor = SystemColors.Control;
            LblGridSize.Location = new Point(691, 426);
            LblGridSize.Name = "LblGridSize";
            LblGridSize.Size = new Size(73, 15);
            LblGridSize.TabIndex = 4;
            LblGridSize.Text = "GridSize: 000";
            // 
            // MainToolbar
            // 
            MainToolbar.Items.AddRange(new ToolStripItem[] { BtnFile, BtnEdit, BtnHelp });
            MainToolbar.Location = new Point(0, 0);
            MainToolbar.Name = "MainToolbar";
            MainToolbar.RenderMode = ToolStripRenderMode.System;
            MainToolbar.Size = new Size(800, 24);
            MainToolbar.TabIndex = 5;
            MainToolbar.Text = "ToolBar";
            // 
            // BtnFile
            // 
            BtnFile.DropDownItems.AddRange(new ToolStripItem[] { BtnFileNew, BtnFileLoad, toolStripSeparator2, BtnFileSave, BtnFileSaveAs });
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
            BtnHelp.DropDownItems.AddRange(new ToolStripItem[] { BtnHelpAbout });
            BtnHelp.Name = "BtnHelp";
            BtnHelp.Size = new Size(44, 20);
            BtnHelp.Text = "Help";
            // 
            // BtnHelpAbout
            // 
            BtnHelpAbout.Name = "BtnHelpAbout";
            BtnHelpAbout.Size = new Size(180, 22);
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
            // MainWindow
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = Color.Black;
            ClientSize = new Size(800, 450);
            Controls.Add(LblGridSize);
            Controls.Add(LblCursor);
            Controls.Add(LblOrigin);
            Controls.Add(BrushPanel);
            Controls.Add(EditorPanel);
            Controls.Add(MainToolbar);
            MainMenuStrip = MainToolbar;
            Name = "MainWindow";
            Text = "Level Editor";
            EditorPanel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize) ImgEditorDraw).EndInit();
            BrushPanel.ResumeLayout(false);
            MainToolbar.ResumeLayout(false);
            MainToolbar.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private Panel EditorPanel;
        private Panel BrushPanel;
        private Label LblOrigin;
        private PictureBox ImgEditorDraw;
        private Label LblCursor;
        private Label LblGridSize;
        private Button BtnLines;
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
    }
}
