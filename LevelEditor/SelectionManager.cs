﻿namespace LevelEditor
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
        Point lastMousePos;
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
                    {
                        if (!sectorDrawer.CheckForNodesInPos(mousePos, 5, out (int, int) foundIndx)) break;
                        if (isCtrlKeyDown) RemoveSelection(new SelectionData(ref foundIndx));
                        else currentSelection.Add(new SelectionData(ref foundIndx));
                    }
                    return;

                case SelectionType.Wall:
                    {
                        if (!sectorDrawer.CheckForWallsInPos(mousePos, 5, out (int, int) foundIndx)) break;
                        if (isCtrlKeyDown) RemoveSelection(new SelectionData(ref foundIndx));
                        else currentSelection.Add(new SelectionData(ref foundIndx));
                        lastMousePos = mousePos;
                    }
                    return;

                case SelectionType.Sector:
                    {
                        if (!sectorDrawer.CheckForSectorsInPos(mousePos, 5, out int foundIndx)) break;
                        if (isCtrlKeyDown) RemoveSelection(new SelectionData(foundIndx, -1));
                        else currentSelection.Add(new SelectionData(foundIndx, -1));
                        lastMousePos = mousePos;
                    }
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
                        int otherWallIndex = ClampWallIndex(i, j - 1);
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
                        int laterWallIndex = ClampWallIndex(i, j - 1), nextWallIndex = ClampWallIndex(i, j + 1);
                        Wall laterWall = ActiveSectors[i].walls[laterWallIndex], nextWall = ActiveSectors[i].walls[nextWallIndex];

                        Point diff = mousePos.Subtract(lastMousePos);

                        laterWall.rightPoint = wall.leftPoint = wall.leftPoint.Add(diff);
                        nextWall.leftPoint = wall.rightPoint = wall.rightPoint.Add(diff);

                        laterWall.UpdateMiddleAndNormal();
                        wall.UpdateMiddleAndNormal();
                        nextWall.UpdateMiddleAndNormal();

                        ActiveSectors[i].walls[laterWallIndex] = laterWall;
                        ActiveSectors[i].walls[j] = wall;
                        ActiveSectors[i].walls[nextWallIndex] = nextWall;
                    });

                    lastMousePos = mousePos;
                    break;

                case SelectionType.Sector:
                    LoopSectors((int i, Sector sector) =>
                    {
                        int size = sector.walls.Count;
                        for (int j = 0; j < size; j++)
                        {
                            Wall wall = sector.walls[j];

                            Point diff = mousePos.Subtract(lastMousePos);
                            wall.leftPoint = wall.leftPoint.Add(diff);
                            wall.rightPoint = wall.rightPoint.Add(diff);

                            wall.UpdateMiddleAndNormal();
                            sector.walls[j] = wall;
                        }

                        ActiveSectors[i] = sector;
                    });

                    lastMousePos = mousePos;
                    break;
            }
        }

        int ClampWallIndex(int currentSector, int indx)
        {
            if (indx < 0) indx = ActiveSectors[currentSector].walls.Count - 1;
            else if (indx >= ActiveSectors[currentSector].walls.Count) indx = 0;
            return indx;
        }

        public void OnMouseUp()
        {
            isMouseDown = false;

            //LoopSectors((int i, Sector sector) =>
            //{
            //    if (sector.CheckWallsOrientation()) sector.FlipWalls();
            //    ActiveSectors[i] = sector;
            //});
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