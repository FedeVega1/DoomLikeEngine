namespace LevelEditor
{
    internal enum SelectionType { Node, Wall, Sector }

    readonly internal struct SelectionData
    {
        public readonly int sectorIndex;
        public readonly int wallIndex;

        public SelectionData(int sectIndx, int wallIndx)
        {
            sectorIndex = sectIndx;
            wallIndex = wallIndx;
        }

        public SelectionData(ref (int, int) indx)
        {
            sectorIndex = indx.Item1;
            wallIndex = indx.Item2;
        }
    }

    internal class SelectionManager
    {
        bool isMouseDown;
        SectorDrawer sectorDrawer;
        Grid grid;

        public SelectionType CurrentSelectionType { get; set; }
        public bool HasSelection => currentSelection.Count > 0;

        List<Sector> ActiveSectors => sectorDrawer.ActiveSectors;
        List<SelectionData> currentSelection;

        public SelectionManager(ref SectorDrawer sectDrawerRef, ref Grid mapGrid)
        {
            grid = mapGrid;
            sectorDrawer = sectDrawerRef;
            currentSelection = new List<SelectionData>();
        }

        public void OnMouseDown(Point mousePos, bool isShiftKeyDown, bool isCtrlKeyDown)
        {
            isMouseDown = true;
            if (!isShiftKeyDown) currentSelection.Clear();

            switch (CurrentSelectionType)
            {
                case SelectionType.Node:
                    if (!sectorDrawer.CheckForNodes(mousePos, 5, out (int, int) foundIndx)) break;
                    if (isCtrlKeyDown) RemoveSelection(new SelectionData(ref foundIndx));
                    else currentSelection.Add(new SelectionData(ref foundIndx));
                    return;

                case SelectionType.Wall:

                    return;

                case SelectionType.Sector:

                    return;
            }

            if (currentSelection.Count == 0) return;
            currentSelection.Clear();
        }

        public void OnMouseMove(Point mousePos)
        {
            if (!isMouseDown) return;
            switch (CurrentSelectionType)
            {
                case SelectionType.Node:
                    LoopWalls((int i, int j, Wall wall) =>
                    {
                        int otherWallIndex = j - 1;
                        if (otherWallIndex < 0) otherWallIndex = ActiveSectors[i].walls.Count - 1;
                        else if (otherWallIndex >= ActiveSectors[i].walls.Count) otherWallIndex = 0;
                        Wall otherWall = ActiveSectors[i].walls[otherWallIndex];

                        otherWall.rightPoint = wall.leftPoint = mousePos;

                        wall.UpdateMiddleAndNormal();
                        otherWall.UpdateMiddleAndNormal();

                        ActiveSectors[i].walls[j] = wall;
                        ActiveSectors[i].walls[otherWallIndex] = otherWall;
                    });
                    break;

                case SelectionType.Wall:
                    LoopWalls((int i, int j, Wall wall) =>
                    {
                        //wall.leftPoint.X += (int) MathF.Round(delta.X);
                        //wall.leftPoint.Y += (int) MathF.Round(delta.Y);

                        //wall.rightPoint.X += (int) MathF.Round(delta.X);
                        //wall.rightPoint.Y += (int) MathF.Round(delta.Y);

                        wall.UpdateMiddleAndNormal();
                        ActiveSectors[i].walls[j] = wall;
                    });
                    break;

                case SelectionType.Sector:
                    LoopSectors((int i, Sector sector) =>
                    {
                        int size = sector.walls.Count;
                        for (int j = 0; j < size; j++)
                        {
                            Wall wall = sector.walls[j];

                            //wall.leftPoint.X += (int) MathF.Round(delta.X);
                            //wall.leftPoint.Y += (int) MathF.Round(delta.Y);

                            //wall.rightPoint.X += (int) MathF.Round(delta.X);
                            //wall.rightPoint.Y += (int) MathF.Round(delta.Y);

                            wall.UpdateMiddleAndNormal();
                            sector.walls[j] = wall;
                        }

                        ActiveSectors[i] = sector;
                    });
                    break;
            }
        }

        public void OnMouseUp()
        {
            isMouseDown = false;
        }

        void RemoveSelection(SelectionData data)
        {
            int size = currentSelection.Count;
            for (int i = 0; i < size; i++)
            {
                if (currentSelection[i].sectorIndex != data.sectorIndex || currentSelection[i].wallIndex != data.wallIndex) continue;
                currentSelection.RemoveAt(i);
                return;
            }
        }

        public void UpdateWallColor(Color newColor)
        {
            if (CurrentSelectionType != SelectionType.Wall) return;

            LoopWalls((int i, int j, Wall wall) =>
            {
                wall.color = newColor;
                ActiveSectors[i].walls[j] = wall;
            });
        }

        public void UpdateSectorFloorColor(Color newColor)
        {
            if (CurrentSelectionType != SelectionType.Sector) return;

            LoopSectors((int i, Sector sector) =>
            {
                sector.ceillingColor = newColor;
                ActiveSectors[i] = sector;
            });
        }

        public void UpdateSectorCeillingColor(Color newColor)
        {
            if (CurrentSelectionType != SelectionType.Sector) return;

            LoopSectors((int i, Sector sector) =>
            {
                sector.ceillingColor = newColor;
                ActiveSectors[i] = sector;
            });
        }

        public void UpdateSectorFloorHeight(int newHeight)
        {
            if (CurrentSelectionType != SelectionType.Sector) return;

            LoopSectors((int i, Sector sector) =>
            {
                sector.floorHeight = newHeight;
                ActiveSectors[i] = sector;
            });
        }

        public void UpdateSectorCeillingHeight(int newHeight)
        {
            if (CurrentSelectionType != SelectionType.Sector) return;

            LoopSectors((int i, Sector sector) =>
            {
                sector.ceillingHeight = newHeight;
                ActiveSectors[i] = sector;
            });
        }

        void LoopSectors(Action<int, Sector> callbackPerSector)
        {
            int size = currentSelection.Count;
            for (int i = 0; i < size; i++)
                callbackPerSector?.Invoke(currentSelection[i].sectorIndex, ActiveSectors[currentSelection[i].sectorIndex]);
        }

        void LoopWalls(Action<int, int, Wall> callbackPerWall)
        {
            int size = currentSelection.Count;
            for (int i = 0; i < size; i++)
            {
                int sector = currentSelection[i].sectorIndex, wall = currentSelection[i].wallIndex;
                callbackPerWall?.Invoke(sector, wall, ActiveSectors[sector].walls[wall]);
            }
        }
    }
}
