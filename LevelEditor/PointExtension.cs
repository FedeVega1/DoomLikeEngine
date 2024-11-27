namespace LevelEditor
{
    static internal class PointExtension
    {
        public static Point Add(this Point left, Point right) => new Point(left.X + right.X, left.Y + right.Y);
        public static Point Add(this Point left, int right) => new Point(left.X + right, left.Y + right);
        public static Point Subtract(this Point left, Point right) => new Point(left.X - right.X, left.Y - right.Y);
        public static Point Subtract(this Point left, int right) => new Point(left.X - right, left.Y - right);
        public static Point Divide(this Point left, int right) => new Point(left.X / right, left.Y / right);
        public static Point Divide(this Point left, float right, bool asd) => new Point((int) MathF.Round((float) left.X / right), (int) MathF.Round((float) left.Y / right));
        public static PointF Divide(this Point left, float right) => new PointF(left.X / right, left.Y / right);
        public static Point Multiply(this Point left, int right) => new Point(left.X * right, left.Y * right);
        public static Point Multiply(this Point left, float right) => new Point((int) MathF.Round(left.X * right), (int) MathF.Round(left.Y * right));
        public static float Magnitude(this Point point) => MathF.Sqrt(point.X * point.X + point.Y * point.Y);
        public static Point Clamp(this Point point, int minX, int maxX, int minY, int maxY) => new Point(Math.Clamp(point.X, minX, maxX), Math.Clamp(point.Y, minY, maxY));

        public static PointF Normalize(this Point point)
        {
            float magnitude = point.Magnitude();
            if (magnitude > .0000001f) return point.Divide(magnitude);
            return PointF.Empty;
        }

        public static float Distance(this Point a, Point b)
        {
            Point diff = new Point(a.X - b.X, a.Y - b.Y);
            return MathF.Sqrt(diff.X * diff.X + diff.Y * diff.Y);
        }

        public static float Dot(this PointF left, PointF right) => left.X * right.X + left.Y * right.Y;
        public static int Cross(this Point left, Point right) => left.X * right.Y - right.X * left.Y;

        public static PointF Add(this PointF left, PointF right) => new PointF(left.X + right.X, left.Y + right.Y);
        public static PointF Add(this PointF left, float right) => new PointF(left.X + right, left.Y + right);
        public static PointF Subtract(this PointF left, PointF right) => new PointF(left.X - right.X, left.Y - right.Y);
        public static PointF Subtract(this PointF left, Point right) => new PointF(left.X - right.X, left.Y - right.Y);
        public static PointF Subtract(this PointF left, float right) => new PointF(left.X - right, left.Y - right);
        public static PointF Divide(this PointF left, float right) => new PointF(left.X / right, left.Y / right);
        public static PointF Multiply(this PointF left, float right) => new PointF(left.X * right, left.Y * right);
        public static float Magnitude(this PointF point) => MathF.Sqrt(point.X * point.X + point.Y * point.Y);
    }
}
