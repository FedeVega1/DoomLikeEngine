namespace LevelEditor
{
    internal class FileManager
    {
        const string MAPVersion = "v00.05", BSPVersion = "v00.07";

        public string CurrentOpenedFile { get; private set; }

        BSPBuilder builder;
        int debug_NumberOfBSPFrontNodes;
        int debug_NumberOfBSPBackNodes;

        public FileManager()
        {
            CurrentOpenedFile = "NULL";
            builder = new BSPBuilder();
        }

        public bool SaveToFile(string fileName, Stream fileStream, ref List<Sector> sectors)
        {
            COLoggerImport.LogNormal("Saving to {0}", fileName);

            int size = sectors.Count;
            COLoggerImport.LogNormal("Number of sectors {0}", size);

            try
            {
                fileStream.Write(ToByteArray(0x1, out int arrSize), 0, arrSize); // endianessCheck

                string[] versionSplit = MAPVersion.Remove(0, 1).Split(".");

                int versionSize = versionSplit.Length;
                fileStream.WriteByte((byte) versionSize);

                for (int i = 0; i < versionSize; i++) fileStream.WriteByte(Convert.ToByte(versionSplit[i]));

                fileStream.Write(ToByteArray(size, out arrSize), 0, arrSize);
                for (int i = 0; i < size; i++)
                {
                    fileStream.Write(ToByteArray(sectors[i].SectorID, out arrSize), 0, arrSize);

                    int count = sectors[i].walls.Count;
                    fileStream.Write(ToByteArray(count, out arrSize), 0, arrSize);

                    for (int j = 0; j < count; j++)
                    {
                        fileStream.Write(ToByteArray(sectors[i].walls[j].leftPoint, out arrSize), 0, arrSize);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].rightPoint, out arrSize), 0, arrSize);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].colors[0], out arrSize), 0, arrSize);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].colors[1], out arrSize), 0, arrSize);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].colors[2], out arrSize), 0, arrSize);
                        byte portalFlags = (byte) ((sectors[i].walls[j].isPortal ? 0b1 : 0) << 1 | (sectors[i].walls[j].isConnection ? 0b1 : 0));
                        fileStream.WriteByte(portalFlags);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].portalTargetSector, out arrSize), 0, arrSize);
                        fileStream.Write(ToByteArray(sectors[i].walls[j].portalTargetWall, out arrSize), 0, arrSize);
                    }

                    fileStream.Write(ToByteArray(sectors[i].ceillingHeight, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].floorHeight, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].floorColor, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].ceillingColor, out arrSize), 0, arrSize);

                    COLoggerImport.LogNormal("Saved sector {0} correct", sectors[i].SectorID);
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

            int intSize = sizeof(int), pointSize = intSize * 2, colorSize = sizeof(byte) * 3, idSize = sizeof(ulong);
            byte[] intBuffer = new byte[intSize], pointBuffer = new byte[pointSize], colorBuffer = new byte[colorSize], idBuffer = new byte[idSize];

            COLoggerImport.LogNormal("Loading File {0}", fileName);

            try
            {
                fileStream.Read(intBuffer, 0, intSize);
                bool isLittleEndian = intBuffer[0] == 0x1;

                string[] split = MAPVersion.Remove(0, 1).Split('.');

                int versionSize = fileStream.ReadByte();
                if (versionSize != split.Length)
                {
                    COLoggerImport.LogError("Bad MAP version size. Expected {0}, got {1}", split.Length, versionSize);
                    return false;
                }

                byte[] versionBuffer = new byte[versionSize];
                fileStream.Read(versionBuffer, 0, versionSize);

                for (int i = 0; i < versionSize; i++)
                {
                    if (versionBuffer[i] == Convert.ToByte(split[i])) continue;
                    COLoggerImport.LogError("Bad MAP version number!");
                    return false;
                }

                COLoggerImport.LogNormal("MAP version Correct!");

                fileStream.Read(intBuffer, 0, intSize);
                int size = ByteArrayToInt(intBuffer, isLittleEndian);
                COLoggerImport.LogNormal("Number of sectors {0} in file", size);

                for (int i = 0; i < size; i++)
                {
                    fileStream.Read(intBuffer, 0, intSize);
                    Sector sector = new Sector(ByteArrayToUInt(intBuffer, isLittleEndian));
                    sector.walls = new List<Wall>();

                    fileStream.Read(intBuffer, 0, intSize);
                    int count = ByteArrayToInt(intBuffer, isLittleEndian);

                    for (int j = 0; j < count; j++)
                    {
                        Wall wall = new Wall(sector);

                        fileStream.Read(pointBuffer, 0, pointSize);
                        wall.leftPoint = ByteArrayToPoint(pointBuffer, isLittleEndian);

                        fileStream.Read(pointBuffer, 0, pointSize);
                        wall.rightPoint = ByteArrayToPoint(pointBuffer, isLittleEndian);

                        fileStream.Read(colorBuffer, 0, colorSize);
                        wall.colors[0] = ByteArrayToColor(colorBuffer);

                        fileStream.Read(colorBuffer, 0, colorSize);
                        wall.colors[1] = ByteArrayToColor(colorBuffer);

                        fileStream.Read(colorBuffer, 0, colorSize);
                        wall.colors[2] = ByteArrayToColor(colorBuffer);

                        int portalFlag = fileStream.ReadByte();
                        wall.isPortal = (portalFlag & 0b10) == 0b10;
                        wall.isConnection = (portalFlag & 0b1) == 0b1;

                        fileStream.Read(intBuffer, 0, intSize);
                        wall.portalTargetSector = ByteArrayToUInt(intBuffer, isLittleEndian);

                        fileStream.Read(idBuffer, 0, idSize);
                        wall.portalTargetWall = ByteArrayToULong(idBuffer, isLittleEndian);

                        sector.walls.Add(wall);
                    }

                    fileStream.Read(intBuffer, 0, intSize);
                    sector.ceillingHeight = ByteArrayToInt(intBuffer, isLittleEndian);

                    fileStream.Read(intBuffer, 0, intSize);
                    sector.floorHeight = ByteArrayToInt(intBuffer, isLittleEndian);

                    fileStream.Read(colorBuffer, 0, colorSize);
                    sector.floorColor = ByteArrayToColor(colorBuffer);

                    fileStream.Read(colorBuffer, 0, colorSize);
                    sector.ceillingColor = ByteArrayToColor(colorBuffer);

                    sectors.Add(sector);
                    COLoggerImport.LogNormal("Load sector {0} correct", sector.SectorID);
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

                string[] versionSplit = BSPVersion.Remove(0, 1).Split(".");

                int versionSize = versionSplit.Length;
                fileStream.WriteByte((byte) versionSize);

                for (int i = 0; i < versionSize; i++) fileStream.WriteByte(Convert.ToByte(versionSplit[i]));

                fileStream.Write(ToByteArray(size, out arrSize), 0, arrSize);
                for (int i = 0; i < size; i++)
                {
                    fileStream.Write(ToByteArray(sectors[i].SectorID, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].ceillingHeight, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].floorHeight, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].floorColor, out arrSize), 0, arrSize);
                    fileStream.Write(ToByteArray(sectors[i].ceillingColor, out arrSize), 0, arrSize);

                    COLoggerImport.LogNormal("Compiled sector {0} correct", sectors[i].SectorID);
                }

                fileStream.Write(ToByteArray(0xAAAAAAAA, out arrSize), 0, arrSize); // Start of BSP tree
                BSPNode bsp = builder.PerformBSP(sectors, out List<SubSector> subSectors);

                int bspNodeCount = 0;
                GetSizeOfBSPTree(bsp, ref bspNodeCount);
                COLoggerImport.LogNormal("BSP Tree Size: {0}", bspNodeCount);
                COLoggerImport.LogNormal("BSP Number of Intersections: {0}", builder.debug_NumberOfIntersections);
                COLoggerImport.LogNormal("BSP Number of Front Nodes: {0}", debug_NumberOfBSPFrontNodes);
                COLoggerImport.LogNormal("BSP Number of Back Nodes: {0}", debug_NumberOfBSPBackNodes);
                COLoggerImport.LogNormal("BSP Selected Splitter Wall: Start {0} | Dir: {1}", bsp.splitter.startPoint, bsp.splitter.dir);

                fileStream.Write(ToByteArray(subSectors.Count, out size), 0, size);
                CompileSubSectors(ref subSectors, fileStream);

                fileStream.Write(ToByteArray(bspNodeCount, out arrSize), 0, arrSize);
                fileStream.WriteByte(0xFF);
                CompileBSP(bsp, fileStream);
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

        void CompileBSPWall(Wall wall, Stream fileStream)
        {
            fileStream.Write(ToByteArray(wall.leftPoint, out int arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(wall.rightPoint, out arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(wall.colors[0], out arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(wall.colors[1], out arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(wall.colors[2], out arrSize), 0, arrSize);
            byte portalFlags = (byte)((wall.isPortal ? 0x1 : 0) << 1 | (wall.isConnection ? 0x1 : 0));
            fileStream.WriteByte(portalFlags);
            fileStream.Write(ToByteArray(wall.portalTargetSector, out arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(wall.portalTargetWall, out arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(wall.wallID, out arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(wall.Sector.SectorID, out arrSize), 0, arrSize);
        }

        void CompileBSPSplitter(Splitter splitter, Stream fileStream)
        {
            fileStream.Write(ToByteArray(splitter.startPoint, out int arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(splitter.segment, out arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(splitter.dir, out arrSize), 0, arrSize);
        }

        void CompileSubSectors(ref List<SubSector> subSectors, Stream fileStream)
        {
            int size = subSectors.Count, arrSize;
            for (int i = 0; i < size; i++)
            {
                fileStream.Write(ToByteArray(subSectors[i].SubSectorID, out arrSize), 0, arrSize);

                int wallCount = subSectors[i].walls.Count;
                fileStream.Write(ToByteArray(wallCount, out arrSize), 0, arrSize);

                for (int w = 0; w < wallCount; w++)
                    CompileBSPWall(subSectors[i].walls[w], fileStream);

                COLoggerImport.LogNormal("Compiled SubSector {0} correct", subSectors[i].SubSectorID);
            }
        }

        void CompileBSP(BSPNode currentNode, Stream fileStream)
        {
            fileStream.Write(ToByteArray(currentNode.NodeID, out int arrSize), 0, arrSize);

            CompileBSPSplitter(currentNode.splitter, fileStream);

            if (currentNode.parentNode != null) fileStream.Write(ToByteArray(currentNode.parentNode.NodeID, out arrSize), 0, arrSize);
            else fileStream.Write(ToByteArray(0xFFFFFFFF, out arrSize), 0, arrSize);

            if (currentNode.subSector != null) fileStream.Write(ToByteArray(currentNode.subSector.SubSectorID, out arrSize), 0, arrSize);
            else fileStream.Write(ToByteArray(0xFFFFFFF, out arrSize), 0, arrSize);

            fileStream.Write(ToByteArray(currentNode.bBox.topPoint, out arrSize), 0, arrSize);
            fileStream.Write(ToByteArray(currentNode.bBox.bottomPoint, out arrSize), 0, arrSize);

            COLoggerImport.LogNormal("Compiled Node {0} correct", currentNode.NodeID);

            if (currentNode.frontNode != null)
            {
                fileStream.WriteByte(0xBB);
                CompileBSP(currentNode.frontNode, fileStream);
            }

            if (currentNode.backNode != null)
            {
                fileStream.WriteByte(0xCC);
                CompileBSP(currentNode.backNode, fileStream);
            }
        }

        void GetSizeOfBSPTree(BSPNode node, ref int count)
        {
            count++;

            if (node.frontNode != null)
            {
                debug_NumberOfBSPFrontNodes++;
                GetSizeOfBSPTree(node.frontNode, ref count);
            }

            if (node.backNode != null)
            {
                debug_NumberOfBSPBackNodes++;
                GetSizeOfBSPTree(node.backNode, ref count);
            }
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

        static byte[] ToByteArray(ulong dataToConvert, out int size)
        {
            size = sizeof(ulong);
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

        static byte[] ToByteArray(uint dataToConvert, out int size)
        {
            size = sizeof(uint);
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

        static byte[] ToByteArray(float dataToConvert, out int size)
        {
            size = sizeof(float);
            byte[] array = new byte[size];

            Buffer.BlockCopy(new float[] { dataToConvert }, 0, array, 0, size);

            if (!BitConverter.IsLittleEndian) Array.Reverse(array);
            return array;
        }

        static byte[] ToByteArray(float dataToConvert)
        {
            int size = sizeof(float);
            byte[] array = new byte[size];

            Buffer.BlockCopy(new float[] { dataToConvert }, 0, array, 0, size);

            if (!BitConverter.IsLittleEndian) Array.Reverse(array);
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

        static byte[] ToByteArray(PointF dataToConvert, out int size)
        {
            int floatSize = sizeof(float);
            size = floatSize * 2;

            byte[] array = new byte[size];
            byte[] x = ToByteArray(dataToConvert.X);
            byte[] y = ToByteArray(dataToConvert.Y);

            for (int i = 0; i < floatSize; i++)
            {
                array[i] = x[i];
                array[i + floatSize] = y[i];
            }

            return array;
        }

        static byte[] ToByteArray(Color dataToConvert, out int size)
        {
            size = 3;
            return [dataToConvert.R, dataToConvert.G, dataToConvert.B];
        }

        static int ByteArrayToInt(byte[] dataToConvert, bool isLittleEndian)
        {
            if (isLittleEndian) return ((int) dataToConvert[3] << 24) | ((int) dataToConvert[2] << 16) | ((int) dataToConvert[1] << 8) | ((int) dataToConvert[0]);
            return ((int) dataToConvert[0] << 24) | ((int) dataToConvert[1] << 16) | ((int) dataToConvert[2] << 8) | ((int) dataToConvert[3]);
        }

        static uint ByteArrayToUInt(byte[] dataToConvert, bool isLittleEndian)
        {
            if (isLittleEndian) return ((uint)dataToConvert[3] << 24) | ((uint)dataToConvert[2] << 16) | ((uint)dataToConvert[1] << 8) | ((uint)dataToConvert[0]);
            return ((uint)dataToConvert[0] << 24) | ((uint)dataToConvert[1] << 16) | ((uint)dataToConvert[2] << 8) | ((uint)dataToConvert[3]);
        }

        static ulong ByteArrayToULong(byte[] dataToConvert, bool isLittleEndian)
        {
            uint high, low;

            if (isLittleEndian)
            {
                high = ((uint) dataToConvert[7] << 24) | ((uint) dataToConvert[6] << 16) | ((uint) dataToConvert[5] << 8) | ((uint) dataToConvert[4]);
                low = ((uint) dataToConvert[3] << 24) | ((uint) dataToConvert[2] << 16) | ((uint) dataToConvert[1] << 8) | ((uint) dataToConvert[0]);
                return ((ulong) high | (ulong) low);
            }

            high = ((uint) dataToConvert[0] << 24) | ((uint) dataToConvert[1] << 16) | ((uint) dataToConvert[2] << 8) | ((uint) dataToConvert[3]);
            low = ((uint) dataToConvert[4] << 24) | ((uint) dataToConvert[5] << 16) | ((uint) dataToConvert[6] << 8) | ((uint) dataToConvert[7]);
            return ((ulong) high | (ulong) low);
        }

        static Point ByteArrayToPoint(byte[] dataToConvert, bool isLittleEndian)
        {
            int x = ByteArrayToInt([dataToConvert[0], dataToConvert[1], dataToConvert[2], dataToConvert[3]], isLittleEndian);
            int y = ByteArrayToInt([dataToConvert[4], dataToConvert[5], dataToConvert[6], dataToConvert[7]], isLittleEndian);
            return new Point(x, y);
        }

        static Color ByteArrayToColor(byte[] dataToConvert) => Color.FromArgb(0xFF, dataToConvert[0], dataToConvert[1], dataToConvert[2]);
    }
}
