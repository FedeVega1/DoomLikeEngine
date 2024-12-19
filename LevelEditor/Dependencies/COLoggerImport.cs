using System.Runtime.InteropServices;

namespace LevelEditor
{
    internal class COLoggerImport
    {
        [DllImport(@"Dependencies\COLogger.dll")]
        public static extern void InitLogSys(bool useConsole, bool logToFile);

        [DllImport(@"Dependencies\COLogger.dll")]
        static extern void Log(int logLevel, char[] message);

        [DllImport(@"Dependencies\COLogger.dll")]
        public static extern void ClearConsole();

        public static void LogVerbose(string message) => Log(0, (message + "\0").ToCharArray());
        public static void LogNormal(string message) => Log(1, (message + "\0").ToCharArray());
        public static void LogWarning(string message) => Log(2, (message + "\0").ToCharArray());
        public static void LogError(string message) => Log(3, (message + "\0").ToCharArray());
        public static void LogCritical(string message) => Log(4, (message + "\0").ToCharArray());

        public static void LogVerbose(string message, params dynamic[] args) => LogVerbose(Format((message + "\0"), args: args));
        public static void LogNormal(string message, params dynamic[] args) => LogNormal(Format((message + "\0"), args: args));
        public static void LogWarning(string message, params dynamic[] args) => LogWarning(Format((message + "\0"), args: args));
        public static void LogError(string message, params dynamic[] args) => LogError(Format((message + "\0"), args: args));
        public static void LogCritical(string message, params dynamic[] args) => LogCritical(Format((message + "\0"), args: args));

        public static void LogVerbose(dynamic obj) => LogVerbose(Format("{0}", args: [obj]));
        public static void LogNormal(dynamic obj) => LogNormal(Format("{0}", args: [obj]));
        public static void LogWarning(dynamic obj) => LogWarning(Format("{0}", args: [obj]));
        public static void LogError(dynamic obj) => LogError(Format("{0}", args: [obj]));
        public static void LogCritical(dynamic obj) => LogCritical(Format("{0}", args: [obj]));

        static string Format(string messageToFormat, int indx = 0, params dynamic[] args)
        {
            if (indx >= args.Length) return messageToFormat;
            string formattedString = "";

            int size = messageToFormat.Length;
            bool onVarFormat = false, appendedArg = false;
            for (int i = 0; i < size; i++)
            {
                if (!appendedArg && messageToFormat[i] == '{')
                {
                    onVarFormat = true;
                    continue;
                }

                if (appendedArg && onVarFormat && messageToFormat[i] == '}')
                {
                    onVarFormat = false;
                    continue;
                }

                if (!onVarFormat)
                {
                    formattedString += messageToFormat[i];
                    continue;
                }

                if (appendedArg) continue;
                formattedString += TypeToString(args[indx++]);

                appendedArg = true;
            }

            if (indx >= args.Length) return formattedString;
            return Format(formattedString, indx, args);
        }

        static string TypeToString(dynamic obj)
        {
            if (obj is int) return ((int) obj).ToString();
            if (obj is string) return ((string) obj).ToString();
            if (obj is byte) return ((byte) obj).ToString();
            if (obj is short) return ((short) obj).ToString();
            if (obj is char) return ((char) obj).ToString();
            if (obj is long) return ((long) obj).ToString();
            if (obj is bool) return ((bool) obj).ToString();
            if (obj is float) return ((float) obj).ToString();
            if (obj is double) return ((double) obj).ToString();
            if (obj is ushort) return ((ushort) obj).ToString();
            if (obj is sbyte) return ((sbyte) obj).ToString();
            if (obj is uint) return ((uint) obj).ToString();
            if (obj is ulong) return ((ulong) obj).ToString();
            if (obj is Point p) return "(" + p.X.ToString() + ", " + p.Y.ToString() + ")";
            if (obj is PointF pf) return "(" + pf.X.ToString() + ", " + pf.Y.ToString() + ")";
            return "Unknow Type";
        }
    }
}
