namespace LevelEditor
{
    static internal class PointExtension
    {
        public static Point Add(this Point left, Point right) => new Point(left.X + right.X, left.Y + right.Y);
        public static Point Add(this Point left, int right) => new Point(left.X + right, left.Y + right);
        public static Point Subtract(this Point left, Point right) => new Point(left.X - right.X, left.Y - right.Y);
        public static Point Subtract(this Point left, int right) => new Point(left.X - right, left.Y - right);
        public static Point Divide(this Point left, int right) => new Point(left.X / right, left.Y / right);
        public static PointF Divide(this Point left, float right) => new PointF(left.X / right, left.Y / right);
        public static Point Multiply(this Point left, int right) => new Point(left.X * right, left.Y * right);
        public static float Magnitude(this Point point) => MathF.Sqrt(point.X * point.X + point.Y * point.Y);
        public static Point Clamp(this Point point, int minX, int maxX, int minY, int maxY) => new Point(Math.Clamp(point.X, minX, maxX), Math.Clamp(point.Y, minY, maxY));

        public static PointF Normalize(this Point point)
        {
            float magnitude = point.Magnitude();
            if (magnitude > .0000001f) return point.Divide(magnitude);
            return PointF.Empty;
        }

        public static float Dot(this PointF left, PointF right) => left.X * right.Y - left.Y * right.X;

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
