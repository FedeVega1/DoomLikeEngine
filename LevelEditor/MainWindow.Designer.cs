
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
            OptionsPanel = new Panel();
            BtnFile = new Button();
            BrushPanel = new Panel();
            LblOrigin = new Label();
            LblCursor = new Label();
            LblGridSize = new Label();
            EditorPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize) ImgEditorDraw).BeginInit();
            OptionsPanel.SuspendLayout();
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
            // OptionsPanel
            // 
            OptionsPanel.BorderStyle = BorderStyle.FixedSingle;
            OptionsPanel.Controls.Add(BtnFile);
            OptionsPanel.Location = new Point(1, 2);
            OptionsPanel.Name = "OptionsPanel";
            OptionsPanel.Size = new Size(797, 33);
            OptionsPanel.TabIndex = 0;
            // 
            // BtnFile
            // 
            BtnFile.Location = new Point(3, 3);
            BtnFile.Name = "BtnFile";
            BtnFile.Size = new Size(75, 23);
            BtnFile.TabIndex = 0;
            BtnFile.Text = "File";
            BtnFile.UseVisualStyleBackColor = true;
            // 
            // BrushPanel
            // 
            BrushPanel.BorderStyle = BorderStyle.FixedSingle;
            BrushPanel.Location = new Point(691, 41);
            BrushPanel.Name = "BrushPanel";
            BrushPanel.Size = new Size(107, 378);
            BrushPanel.TabIndex = 1;
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
            Controls.Add(OptionsPanel);
            Controls.Add(EditorPanel);
            Name = "MainWindow";
            Text = "Form1";
            EditorPanel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize) ImgEditorDraw).EndInit();
            OptionsPanel.ResumeLayout(false);
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private Panel EditorPanel;
        private Panel OptionsPanel;
        private Button BtnFile;
        private Panel BrushPanel;
        private Label LblOrigin;
        private PictureBox ImgEditorDraw;
        private Label LblCursor;
        private Label LblGridSize;
    }
}
