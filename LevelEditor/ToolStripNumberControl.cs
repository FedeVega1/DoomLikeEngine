using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.Design;

namespace LevelEditor
{
    [ToolStripItemDesignerAvailability(ToolStripItemDesignerAvailability.ToolStrip)]
    public class ToolStripNumberControl : ToolStripControlHost
    {
        public event EventHandler ValueChanged;
        public NumericUpDown? NumericUpDownControl => Control as NumericUpDown;

        public ToolStripNumberControl() : base(new NumericUpDown())
        {

        }

        protected override void OnSubscribeControlEvents(Control? control)
        {
            base.OnSubscribeControlEvents(control);
            if (control == null) return;
            ((NumericUpDown) control).ValueChanged += new EventHandler(OnValueChanged);
        }

        protected override void OnUnsubscribeControlEvents(Control? control)
        {
            base.OnSubscribeControlEvents(control);
            if (control == null) return;
            ((NumericUpDown) control).ValueChanged -= new EventHandler(OnValueChanged);
        }

        public void OnValueChanged(object? sender, EventArgs e) => ValueChanged?.Invoke(this, e);
    }
}
