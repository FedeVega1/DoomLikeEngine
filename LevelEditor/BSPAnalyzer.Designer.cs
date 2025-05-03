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
            tableLayoutPanel1 = new TableLayoutPanel();
            lblPartition = new Label();
            lblCurrentNode = new Label();
            lblProcess = new Label();
            nodeTree = new TreeView();
            ((System.ComponentModel.ISupportInitialize)imgMainPreview).BeginInit();
            tableLayoutPanel1.SuspendLayout();
            SuspendLayout();
            // 
            // imgMainPreview
            // 
            imgMainPreview.BackColor = Color.FromArgb(35, 35, 35);
            imgMainPreview.Dock = DockStyle.Fill;
            imgMainPreview.Location = new Point(0, 0);
            imgMainPreview.Name = "imgMainPreview";
            imgMainPreview.Size = new Size(979, 498);
            imgMainPreview.TabIndex = 0;
            imgMainPreview.TabStop = false;
            imgMainPreview.Paint += ImgMainPreview_Paint;
            imgMainPreview.Resize += ImgMainPreview_Resize;
            // 
            // tableLayoutPanel1
            // 
            tableLayoutPanel1.BackColor = Color.FromArgb(35, 35, 35);
            tableLayoutPanel1.ColumnCount = 1;
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            tableLayoutPanel1.Controls.Add(lblPartition, 0, 2);
            tableLayoutPanel1.Controls.Add(lblCurrentNode, 0, 1);
            tableLayoutPanel1.Controls.Add(lblProcess, 0, 0);
            tableLayoutPanel1.Controls.Add(nodeTree, 0, 4);
            tableLayoutPanel1.Enabled = false;
            tableLayoutPanel1.Location = new Point(866, 0);
            tableLayoutPanel1.Name = "tableLayoutPanel1";
            tableLayoutPanel1.RowCount = 5;
            tableLayoutPanel1.RowStyles.Add(new RowStyle());
            tableLayoutPanel1.RowStyles.Add(new RowStyle());
            tableLayoutPanel1.RowStyles.Add(new RowStyle());
            tableLayoutPanel1.RowStyles.Add(new RowStyle());
            tableLayoutPanel1.RowStyles.Add(new RowStyle());
            tableLayoutPanel1.Size = new Size(113, 498);
            tableLayoutPanel1.TabIndex = 1;
            tableLayoutPanel1.Visible = false;
            // 
            // lblPartition
            // 
            lblPartition.AutoSize = true;
            lblPartition.Dock = DockStyle.Fill;
            lblPartition.Font = new Font("Segoe UI", 12F);
            lblPartition.ForeColor = SystemColors.Window;
            lblPartition.Location = new Point(3, 84);
            lblPartition.Name = "lblPartition";
            lblPartition.Size = new Size(107, 21);
            lblPartition.TabIndex = 2;
            lblPartition.Text = "Partition";
            lblPartition.TextAlign = ContentAlignment.MiddleCenter;
            // 
            // lblCurrentNode
            // 
            lblCurrentNode.AutoSize = true;
            lblCurrentNode.Dock = DockStyle.Fill;
            lblCurrentNode.Font = new Font("Segoe UI", 12F);
            lblCurrentNode.ForeColor = SystemColors.Window;
            lblCurrentNode.Location = new Point(3, 42);
            lblCurrentNode.Name = "lblCurrentNode";
            lblCurrentNode.Size = new Size(107, 42);
            lblCurrentNode.TabIndex = 1;
            lblCurrentNode.Text = "CurrentNode: 0";
            lblCurrentNode.TextAlign = ContentAlignment.MiddleCenter;
            // 
            // lblProcess
            // 
            lblProcess.AutoSize = true;
            lblProcess.Dock = DockStyle.Fill;
            lblProcess.Font = new Font("Segoe UI", 12F);
            lblProcess.ForeColor = SystemColors.Window;
            lblProcess.Location = new Point(3, 0);
            lblProcess.Name = "lblProcess";
            lblProcess.Size = new Size(107, 42);
            lblProcess.TabIndex = 0;
            lblProcess.Text = "Current Process";
            lblProcess.TextAlign = ContentAlignment.MiddleCenter;
            // 
            // nodeTree
            // 
            nodeTree.Dock = DockStyle.Fill;
            nodeTree.Location = new Point(3, 108);
            nodeTree.Name = "nodeTree";
            nodeTree.Size = new Size(107, 432);
            nodeTree.TabIndex = 3;
            // 
            // BSPAnalyzer
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(979, 498);
            Controls.Add(tableLayoutPanel1);
            Controls.Add(imgMainPreview);
            Name = "BSPAnalyzer";
            Text = "BSPAnalyzer";
            ((System.ComponentModel.ISupportInitialize)imgMainPreview).EndInit();
            tableLayoutPanel1.ResumeLayout(false);
            tableLayoutPanel1.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private PictureBox imgMainPreview;
        private TableLayoutPanel tableLayoutPanel1;
        private Label lblCurrentNode;
        private Label lblProcess;
        private Label lblPartition;
        private TreeView nodeTree;
    }
}