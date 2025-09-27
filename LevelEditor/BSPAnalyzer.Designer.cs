namespace LevelEditor
{
    partial class BSPAnalyzer
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            imgMainPreview = new PictureBox();
            splitContainer1 = new SplitContainer();
            tableLayoutPanel1 = new TableLayoutPanel();
            btnPlay = new Button();
            cbSubSectors = new CheckBox();
            cbSplitters = new CheckBox();
            cbBBox = new CheckBox();
            ((System.ComponentModel.ISupportInitialize) imgMainPreview).BeginInit();
            ((System.ComponentModel.ISupportInitialize) splitContainer1).BeginInit();
            splitContainer1.Panel1.SuspendLayout();
            splitContainer1.Panel2.SuspendLayout();
            splitContainer1.SuspendLayout();
            tableLayoutPanel1.SuspendLayout();
            SuspendLayout();
            // 
            // imgMainPreview
            // 
            imgMainPreview.BackColor = Color.FromArgb(  35,   35,   35);
            imgMainPreview.Dock = DockStyle.Fill;
            imgMainPreview.Location = new Point(0, 0);
            imgMainPreview.Name = "imgMainPreview";
            imgMainPreview.Size = new Size(849, 498);
            imgMainPreview.TabIndex = 0;
            imgMainPreview.TabStop = false;
            imgMainPreview.Paint += ImgMainPreview_Paint;
            imgMainPreview.Resize += ImgMainPreview_Resize;
            // 
            // splitContainer1
            // 
            splitContainer1.Dock = DockStyle.Fill;
            splitContainer1.IsSplitterFixed = true;
            splitContainer1.Location = new Point(0, 0);
            splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            splitContainer1.Panel1.Controls.Add(imgMainPreview);
            // 
            // splitContainer1.Panel2
            // 
            splitContainer1.Panel2.Controls.Add(tableLayoutPanel1);
            splitContainer1.Size = new Size(979, 498);
            splitContainer1.SplitterDistance = 849;
            splitContainer1.TabIndex = 1;
            // 
            // tableLayoutPanel1
            // 
            tableLayoutPanel1.ColumnCount = 1;
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            tableLayoutPanel1.Controls.Add(btnPlay, 0, 3);
            tableLayoutPanel1.Controls.Add(cbSubSectors, 0, 0);
            tableLayoutPanel1.Controls.Add(cbSplitters, 0, 1);
            tableLayoutPanel1.Controls.Add(cbBBox, 0, 2);
            tableLayoutPanel1.Dock = DockStyle.Fill;
            tableLayoutPanel1.Location = new Point(0, 0);
            tableLayoutPanel1.Name = "tableLayoutPanel1";
            tableLayoutPanel1.RowCount = 4;
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 25F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 25F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 25F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 25F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Absolute, 20F));
            tableLayoutPanel1.Size = new Size(126, 498);
            tableLayoutPanel1.TabIndex = 0;
            // 
            // btnPlay
            // 
            btnPlay.Dock = DockStyle.Fill;
            btnPlay.Location = new Point(3, 375);
            btnPlay.Name = "btnPlay";
            btnPlay.Size = new Size(120, 120);
            btnPlay.TabIndex = 0;
            btnPlay.Text = "PlayGeneration";
            btnPlay.UseVisualStyleBackColor = true;
            // 
            // cbSubSectors
            // 
            cbSubSectors.AutoSize = true;
            cbSubSectors.Checked = true;
            cbSubSectors.CheckState = CheckState.Checked;
            cbSubSectors.Dock = DockStyle.Fill;
            cbSubSectors.Location = new Point(3, 3);
            cbSubSectors.Name = "cbSubSectors";
            cbSubSectors.Size = new Size(120, 118);
            cbSubSectors.TabIndex = 1;
            cbSubSectors.Text = "Show SubSectors";
            cbSubSectors.UseVisualStyleBackColor = true;
            cbSubSectors.CheckedChanged += OnCheckBoxChanged;
            // 
            // cbSplitters
            // 
            cbSplitters.AutoSize = true;
            cbSplitters.Checked = true;
            cbSplitters.CheckState = CheckState.Checked;
            cbSplitters.Dock = DockStyle.Fill;
            cbSplitters.Location = new Point(3, 127);
            cbSplitters.Name = "cbSplitters";
            cbSplitters.Size = new Size(120, 118);
            cbSplitters.TabIndex = 2;
            cbSplitters.Text = "Show Splitters";
            cbSplitters.UseVisualStyleBackColor = true;
            cbSplitters.CheckedChanged += OnCheckBoxChanged;
            // 
            // cbBBox
            // 
            cbBBox.AutoSize = true;
            cbBBox.Checked = true;
            cbBBox.CheckState = CheckState.Checked;
            cbBBox.Dock = DockStyle.Fill;
            cbBBox.Location = new Point(3, 251);
            cbBBox.Name = "cbBBox";
            cbBBox.Size = new Size(120, 118);
            cbBBox.TabIndex = 3;
            cbBBox.Text = "Show Bbox";
            cbBBox.UseVisualStyleBackColor = true;
            cbBBox.CheckedChanged += OnCheckBoxChanged;
            // 
            // BSPAnalyzer
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(979, 498);
            Controls.Add(splitContainer1);
            Name = "BSPAnalyzer";
            Text = "BSPAnalyzer";
            ((System.ComponentModel.ISupportInitialize) imgMainPreview).EndInit();
            splitContainer1.Panel1.ResumeLayout(false);
            splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize) splitContainer1).EndInit();
            splitContainer1.ResumeLayout(false);
            tableLayoutPanel1.ResumeLayout(false);
            tableLayoutPanel1.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private PictureBox imgMainPreview;
        private SplitContainer splitContainer1;
        private TableLayoutPanel tableLayoutPanel1;
        private Button btnPlay;
        private CheckBox cbSubSectors;
        private CheckBox cbSplitters;
        private CheckBox cbBBox;
    }
}