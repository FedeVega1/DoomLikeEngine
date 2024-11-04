namespace LevelEditor
{
    internal class FileManager
    {
        public string CurrentOpenedFile { get; private set; }

        public FileManager()
        {
            CurrentOpenedFile = "NULL";
        }

        public bool SaveToFile(string fileName, Stream fileStream, ref List<Sector> sectors)
        {
            COLoggerImport.LogNormal("Saving to {0}", fileName);

            int size = sectors.Count;
            COLoggerImport.LogNormal("Number of sectors {0}", size);

            try
            {
                fileStream.Write(ToByteArray(size, out int arrSize), 0, arrSize);

                for (int i = 0; i < size; i++)
                {
                    int count = sectors[i].walls.Count;
                    fileStream.Write(ToByteArray(count, out arrSize), 0, arrSize);

                    for (int j = 0; j < count; j++)
                    {
                        fileStream.Write(ToByteArray(sectors[i].walls[j].leftPoint, out arrSize), 0, arrSize);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].rightPoint, out arrSize), 0, arrSize);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].color, out arrSize), 0, arrSize);
                    }

                    fileStream.Write(ToByteArray(sectors[i].ceillingHeight, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].floorHeight, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].floorColor, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].ceillingColor, out arrSize), 0, arrSize);

                    COLoggerImport.LogNormal("Saved sector {0} correct", i);
                }
            }
            catch (Exception e)
            {
                COLoggerImport.LogError("Error while trying to save file {0}", fileName);
                COLoggerImport.LogError(e.Message);
                if (e.StackTrace != null) COLoggerImport.LogError(e.StackTrace);
                return false;
            }

            COLoggerImport.LogNormal("Save Correct!");
            CurrentOpenedFile = fileName;
            return true;
        }

        public bool LoadFromFile(string fileName, Stream fileStream, out List<Sector> sectors)
        {
            sectors = new List<Sector>();

            int intSize = sizeof(int), pointSize = intSize * 2, colorSize = sizeof(byte) * 3;

            byte[] intBuffer = new byte[intSize];
            byte[] pointBuffer = new byte[pointSize];
            byte[] colorBuffer = new byte[colorSize];

            COLoggerImport.LogNormal("loading File {0}", fileName);

            try
            {
                fileStream.Read(intBuffer, 0, intSize);
                int size = ByteArrayToInt(intBuffer);
                COLoggerImport.LogNormal("Number of sectors {0} in file", size);

                for (int i = 0; i < size; i++)
                {
                    Sector sector = new Sector();
                    sector.walls = new List<Wall>();

                    fileStream.Read(intBuffer, 0, intSize);
                    int count = ByteArrayToInt(intBuffer);

                    for (int j = 0; j < count; j++)
                    {
                        Wall wall = new Wall();

                        fileStream.Read(pointBuffer, 0, pointSize);
                        wall.leftPoint = ByteArrayToPoint(pointBuffer);

                        fileStream.Read(pointBuffer, 0, pointSize);
                        wall.rightPoint = ByteArrayToPoint(pointBuffer);

                        fileStream.Read(colorBuffer, 0, colorSize);
                        wall.color = ByteArrayToColor(colorBuffer);

                        sector.walls.Add(wall);
                    }

                    fileStream.Read(intBuffer, 0, intSize);
                    sector.ceillingHeight = ByteArrayToInt(intBuffer);

                    fileStream.Read(intBuffer, 0, intSize);
                    sector.floorHeight = ByteArrayToInt(intBuffer);

                    fileStream.Read(colorBuffer, 0, colorSize);
                    sector.floorColor = ByteArrayToColor(colorBuffer);

                    fileStream.Read(colorBuffer, 0, colorSize);
                    sector.ceillingColor = ByteArrayToColor(colorBuffer);

                    sectors.Add(sector);
                    COLoggerImport.LogNormal("Load sector {0} correct", i);
                }
            }
            catch (Exception e)
            {
                COLoggerImport.LogError("Error while trying to open file {0}", fileName);
                COLoggerImport.LogError(e.Message);
                if (e.StackTrace != null) COLoggerImport.LogError(e.StackTrace);
                return false;
            }

            CurrentOpenedFile = fileName;
            COLoggerImport.LogNormal("Load Correct!");
            return true;
        }

        public bool CompileMap(string fileName, Stream fileStream, ref List<Sector> sectors)
        {
            COLoggerImport.LogNormal("Compiling to {0}", fileName);

            int size = sectors.Count;
            COLoggerImport.LogNormal("Number of sectors {0}", size);

            try
            {
                fileStream.Write(ToByteArray(0x1, out int arrSize), 0, arrSize); // endianessCheck

                fileStream.Write(ToByteArray(size, out arrSize), 0, arrSize);

                for (int i = 0; i < size; i++)
                {
                    int count = sectors[i].walls.Count;
                    fileStream.Write(ToByteArray(count, out arrSize), 0, arrSize);

                    for (int j = 0; j < count; j++)
                    {
                        fileStream.Write(ToByteArray(sectors[i].walls[j].leftPoint, out arrSize), 0, arrSize);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].rightPoint, out arrSize), 0, arrSize);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].color, out arrSize), 0, arrSize);
                    }

                    fileStream.Write(ToByteArray(sectors[i].ceillingHeight, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].floorHeight, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].floorColor, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].ceillingColor, out arrSize), 0, arrSize);

                    COLoggerImport.LogNormal("Compiled sector {0} correct", i);
                }
            }
            catch (Exception e)
            {
                COLoggerImport.LogError("Error while trying to save to file {0}", fileName);
                COLoggerImport.LogError(e.Message);
                if (e.StackTrace != null) COLoggerImport.LogError(e.StackTrace);
                return false;
            }

            COLoggerImport.LogNormal("Compile Correct!");
            return true;
        }

        public void ResetCurrentFile() => CurrentOpenedFile = "NULL";

        static byte[] ToByteArray(int dataToConvert, out int size)
        {
            size = sizeof(int);
            byte[] array = new byte[size];

            if (BitConverter.IsLittleEndian)
            {
                for (int i = 0; i < size; i++)
                    array[i] = (byte) ((dataToConvert >> (8 * i)) & 0xFF);
            }
            else
            {
                for (int i = size - 1, n = 0; i >= 0; i--, n++)
                    array[n] = (byte) ((dataToConvert >> (8 * i)) & 0xFF);
            }

            return array;
        }

        static byte[] ToByteArray(int dataToConvert)
        {
            int size = sizeof(int);
            byte[] array = new byte[size];

            for (int i = 0; i < size; i++)
                array[i] = (byte) ((dataToConvert >> (8 * i)) & 0xFF);

            return array;
        }

        static byte[] ToByteArray(Point dataToConvert, out int size)
        {
            int intSize = sizeof(int);
            size = intSize * 2;

            byte[] array = new byte[size];
            byte[] x = ToByteArray(dataToConvert.X);
            byte[] y = ToByteArray(dataToConvert.Y);

            for (int i = 0; i < intSize; i++)
            {
                array[i] = x[i];
                array[i + intSize] = y[i];
            }

            return array;
        }

        static byte[] ToByteArray(Color dataToConvert, out int size)
        {
            size = 3;
            return [dataToConvert.R, dataToConvert.G, dataToConvert.B];
        }

        static int ByteArrayToInt(byte[] dataToConvert) => (int) dataToConvert[3] << 24 | (int) dataToConvert[2] << 16 | (int) dataToConvert[1] << 8 | (int) dataToConvert[0];

        static Point ByteArrayToPoint(byte[] dataToConvert) => new Point(ByteArrayToInt([dataToConvert[0], dataToConvert[1], dataToConvert[2], dataToConvert[3]]), ByteArrayToInt([dataToConvert[4], dataToConvert[5], dataToConvert[6], dataToConvert[7]]));
        static Color ByteArrayToColor(byte[] dataToConvert) => Color.FromArgb(0xFF, dataToConvert[0], dataToConvert[1], dataToConvert[2]);
    }
}
