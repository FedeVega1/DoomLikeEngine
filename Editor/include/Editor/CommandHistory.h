#pragma once

namespace Editor
{
	class MapData;

	class IEditorCommand
	{
	public:
		virtual ~IEditorCommand() = default;
		virtual void Execute(MapData&) = 0;
		virtual void Undo(MapData&) = 0;
		virtual std::string GetDescription() const = 0;
		virtual std::optional<GUID> GetRestoredLineTarget() const { return std::nullopt; }
		virtual std::optional<GUID> GetRestoredUndoTarget() const { return std::nullopt; }
		virtual std::optional<GUID> GetRestoredWallID() const { return std::nullopt; }
	};

	class CommandHistory
	{
	public:
		void Push(std::unique_ptr<IEditorCommand> cmd)
		{
			undoStack.push_back(std::move(cmd));
			redoStack.clear();
		}

		void Undo(MapData& map);
		void Redo(MapData& map);

		void Clear()
		{
			undoStack.clear();
			redoStack.clear();
		}

		bool CanUndo() const { return !undoStack.empty(); }
		bool CanRedo() const { return !redoStack.empty(); }

		std::optional<std::reference_wrapper<const IEditorCommand>> PeekUndo() const
		{
			if (undoStack.empty()) return std::nullopt;
			return std::cref(*undoStack.back());
		}
		std::optional<std::reference_wrapper<const IEditorCommand>> PeekRedo() const
		{
			if (redoStack.empty()) return std::nullopt;
			return std::cref(*redoStack.back());
		}
		const std::vector<std::unique_ptr<IEditorCommand>>& GetUndoStack() const { return undoStack; }

	private:
		std::vector<std::unique_ptr<IEditorCommand>> undoStack;
		std::vector<std::unique_ptr<IEditorCommand>> redoStack;
	};
}
