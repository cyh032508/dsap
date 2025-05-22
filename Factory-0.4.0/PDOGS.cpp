#ifndef PDOGS_HPP
#define PDOGS_HPP
#include <iostream>
#include <memory>
#include <random>
#include <queue>
#include <functional>
#include <cassert>
#include <set>
#include <array>
#include <string>

namespace Feis
{
    struct GameManagerConfig
    {
        static constexpr int kBoardWidth = 62;
        static constexpr int kBoardHeight = 36;
        static constexpr std::size_t kGoalSize = 4;
        static constexpr std::size_t kConveyorBufferSize = 10;
        static constexpr int kNumberOfWalls = 100;
        static constexpr std::size_t kEndTime = 9000;
    };

    struct CellPosition
    {
        int row;
        int col;
        CellPosition &operator+=(const CellPosition &other)
        {
            row += other.row;
            col += other.col;
            return *this;
        }
    };

    bool operator==(const CellPosition &lhs, const CellPosition &rhs)
    {
        return lhs.row == rhs.row && lhs.col == rhs.col;
    }

    bool operator!=(const CellPosition &lhs, const CellPosition &rhs)
    {
        return !(lhs == rhs);
    }

    CellPosition operator+(const CellPosition &lhs, const CellPosition &rhs)
    {
        return {lhs.row + rhs.row, lhs.col + rhs.col};
    }

    enum class Direction
    {
        kTop = 0,
        kRight = 1,
        kBottom = 2,
        kLeft = 3
    };

    class GameBoard;

    class LayeredCell;

    class IGameInfo
    {
    public:
        virtual std::string GetLevelInfo() const = 0;
        virtual const LayeredCell &GetLayeredCell(CellPosition cellPosition) const = 0;
        virtual bool IsScoredProduct(int number) const = 0;
        virtual int GetScores() const = 0;
        virtual int GetEndTime() const = 0;
        virtual int GetElapsedTime() const = 0;
        virtual bool IsGameOver() const = 0;
    };

    class IGameManager : public IGameInfo
    {
    public:
        virtual void OnProductReceived(int number) = 0;
    };

    class Cell;

    class NumberCell;
    class CollectionCenterCell;
    class MiningMachineCell;
    class ConveyorCell;
    class CombinerCell;
    class WallCell;

    class CellVisitor
    {
    public:
        virtual void Visit(const NumberCell *cell) const {}
        virtual void Visit(const CollectionCenterCell *cell) const {}
        virtual void Visit(const MiningMachineCell *cell) const {}
        virtual void Visit(const ConveyorCell *cell) const {}
        virtual void Visit(const CombinerCell *cell) const {}
        virtual void Visit(const WallCell *cell) const {}
    };

    class Cell
    {
    public:
        virtual void Accept(const CellVisitor *visitor) const = 0;
        virtual ~Cell() {}
    };

    class IBackgroundCell : public Cell
    {
    public:
        virtual bool CanBuild() const = 0;
        virtual ~IBackgroundCell() {}
    };

    class ForegroundCell : public Cell
    {
    public:
        ForegroundCell(CellPosition topLeftCellPosition) : topLeftCellPosition_(topLeftCellPosition) {}

        virtual std::size_t GetWidth() const { return 1; }

        virtual std::size_t GetHeight() const { return 1; }

        virtual CellPosition GetTopLeftCellPosition() const { return topLeftCellPosition_; }

        virtual bool CanRemove() const { return false; }

        virtual std::size_t GetCapacity(CellPosition cellPosition) const { return 0; }

        virtual void ReceiveProduct(CellPosition cellPosition, int number) {}

        virtual void UpdatePassOne(CellPosition cellPosition, GameBoard &board) {}

        virtual void UpdatePassTwo(CellPosition cellPosition, GameBoard &board) {}

        virtual ~ForegroundCell() {}

    protected:
        CellPosition topLeftCellPosition_;
    };

    class ICellRenderer
    {
    public:
        virtual void RenderPassOne(CellPosition position) const = 0;
        virtual void RenderPassTwo(CellPosition position) const = 0;
    };

    CellPosition GetNeighborCellPosition(CellPosition cellPosition, Direction direction)
    {
        switch (direction)
        {
        case Direction::kTop:
            return cellPosition + CellPosition{-1, 0};
        case Direction::kRight:
            return cellPosition + CellPosition{0, 1};
        case Direction::kBottom:
            return cellPosition + CellPosition{1, 0};
        case Direction::kLeft:
            return cellPosition + CellPosition{0, -1};
        }
        assert(false);
        return {0, 0};
    }

    std::size_t GetNeighborCapacity(const GameBoard &board, CellPosition cellPosition, Direction direction);

    void SendProduct(GameBoard &board, CellPosition cellPosition, Direction direction, int product);

    class ConveyorCell : public ForegroundCell
    {
    public:
        ConveyorCell(CellPosition topLeftCellPosition, Direction direction)
            : ForegroundCell(topLeftCellPosition), products_{}, direction_{direction} {}

        int GetProduct(std::size_t i) const { return products_[i]; }

        std::size_t GetProductCount() const { return products_.size(); }

        Direction GetDirection() const { return direction_; }

        void Accept(const CellVisitor *visitor) const override
        {
            visitor->Visit(this);
        }

        bool CanRemove() const override
        {
            return true;
        }

        std::size_t GetCapacity(CellPosition cellPosition) const override
        {
            for (std::size_t i = 0; i < products_.size(); ++i)
            {
                if (products_[products_.size() - 1 - i] != 0)
                {
                    return i;
                }
            }
            return products_.size();
        }

        void ReceiveProduct(CellPosition cellPosition, int number) override
        {
            assert(number != 0);
            assert(products_.back() == 0);
            products_.back() = number;
        }

        void UpdatePassOne(CellPosition cellPosition, GameBoard &board) override
        {
            std::size_t capacity = GetNeighborCapacity(board, cellPosition, direction_);

            if (capacity >= 3)
            {
                if (products_[0] != 0)
                {
                    SendProduct(board, cellPosition, direction_, products_[0]);
                    products_[0] = 0;
                }
            }

            if (capacity >= 2)
            {
                if (products_[0] == 0 && products_[1] != 0)
                {
                    std::swap(products_[0], products_[1]);
                }
            }

            if (capacity >= 1)
            {
                if (products_[0] == 0 && products_[1] == 0 && products_[2] != 0)
                {
                    std::swap(products_[1], products_[2]);
                }
            }
        }

        void UpdatePassTwo(CellPosition cellPosition, GameBoard &board) override
        {
            for (std::size_t k = 3; k < products_.size(); ++k)
            {
                if (products_[k] != 0 && products_[k - 1] == 0 && products_[k - 2] == 0 && products_[k - 3] == 0)
                {
                    std::swap(products_[k], products_[k - 1]);
                }
            }
        }

    protected:
        std::array<int, GameManagerConfig::kConveyorBufferSize> products_;

    private:
        Direction direction_;
    };

    class CombinerCell : public ForegroundCell
    {
    public:
        CombinerCell(CellPosition topLeft, Direction direction)
            : ForegroundCell(topLeft), direction_{direction}, firstSlotProduct_{}, secondSlotProduct_{} {}

        Direction GetDirection() const { return direction_; }

        int GetFirstSlotProduct() const { return firstSlotProduct_; }

        int GetSecondSlotProduct() const { return secondSlotProduct_; }

        void Accept(const CellVisitor *visitor) const override
        {
            visitor->Visit(this);
        }

        std::size_t GetWidth() const override
        {
            return direction_ == Direction::kTop || direction_ == Direction::kBottom ? 2 : 1;
        }

        std::size_t GetHeight() const override
        {
            return direction_ == Direction::kTop || direction_ == Direction::kBottom ? 1 : 2;
        }

        bool CanRemove() const override
        {
            return true;
        }

        bool IsMainCell(CellPosition cellPosition) const
        {
            switch (direction_)
            {
            case Direction::kTop:
            case Direction::kRight:
                return cellPosition != ForegroundCell::topLeftCellPosition_;
            case Direction::kBottom:
            case Direction::kLeft:
                return cellPosition == ForegroundCell::topLeftCellPosition_;
            }
            assert(false);
            return false;
        }

        std::size_t GetCapacity(CellPosition cellPosition) const override
        {
            if (IsMainCell(cellPosition))
            {
                if (firstSlotProduct_ == 0)
                {
                    return GameManagerConfig::kConveyorBufferSize;
                }
                return 0;
            }

            if (secondSlotProduct_ == 0)
            {
                return GameManagerConfig::kConveyorBufferSize;
            }
            return 0;
        }

        void ReceiveProduct(CellPosition cellPosition, int number) override
        {
            assert(number != 0);

            if (IsMainCell(cellPosition))
            {
                firstSlotProduct_ = number;
            }
            else
            {
                secondSlotProduct_ = number;
            }
        }

        void UpdatePassOne(CellPosition cellPosition, GameBoard &board) override
        {
            if (!IsMainCell(cellPosition))
                return;

            if (firstSlotProduct_ != 0 && secondSlotProduct_ != 0)
            {
                if (GetNeighborCapacity(board, cellPosition, direction_) >= 3)
                {
                    SendProduct(board, cellPosition, direction_, firstSlotProduct_ + secondSlotProduct_);
                    firstSlotProduct_ = 0;
                    secondSlotProduct_ = 0;
                }
            }
        }

    private:
        Direction direction_;
        int firstSlotProduct_;
        int secondSlotProduct_;
    };

    class WallCell : public ForegroundCell
    {
    public:
        WallCell(CellPosition topLeft) : ForegroundCell(topLeft) {}

        bool CanRemove() const override
        {
            return false;
        }
        void Accept(const CellVisitor *visitor) const override
        {
            visitor->Visit(this);
        }
    };

    class CollectionCenterCell : public ForegroundCell
    {
    public:
        CollectionCenterCell(
            CellPosition topLeft,
            IGameManager *gameManager)
            : ForegroundCell(topLeft),
              gameManager_{gameManager} {}

        void Accept(const CellVisitor *visitor) const override
        {
            visitor->Visit(this);
        }

        std::size_t GetWidth() const override
        {
            return GameManagerConfig::kGoalSize;
        }
        std::size_t GetHeight() const override
        {
            return GameManagerConfig::kGoalSize;
        }
        std::size_t GetCapacity(CellPosition cellPosition) const override
        {
            return GameManagerConfig::kConveyorBufferSize;
        }
        void ReceiveProduct(CellPosition cellPosition, int number) override
        {
            assert(number != 0);

            if (number)
            {
                gameManager_->OnProductReceived(number);
            }
        }
        int GetScores() const
        {
            return gameManager_->GetScores();
        }

    private:
        IGameManager *gameManager_;
    };

    class LayeredCell
    {
    public:
        std::shared_ptr<ForegroundCell> GetForeground() const
        {
            return foreground_;
        }
        std::shared_ptr<IBackgroundCell> GetBackground() const
        {
            return background_;
        }
        bool CanBuild() const
        {
            return foreground_ == nullptr &&
                   (background_ == nullptr || background_->CanBuild());
        }

        void SetForegrund(const std::shared_ptr<ForegroundCell> &value)
        {
            foreground_ = value;
        }
        void SetBackground(const std::shared_ptr<IBackgroundCell> &value)
        {
            background_ = value;
        }

    private:
        std::shared_ptr<ForegroundCell> foreground_;
        std::shared_ptr<IBackgroundCell> background_;
    };

    class GameBoard
    {
    public:
        const LayeredCell &GetLayeredCell(CellPosition cellPosition) const
        {
            return layeredCells_[cellPosition.row][cellPosition.col];
        }

        bool CanBuild(const std::shared_ptr<ForegroundCell> &cell)
        {
            if (cell == nullptr)
            {
                return false;
            }

            CellPosition cellPosition = cell->GetTopLeftCellPosition();

            if (cellPosition.col < 0 || cellPosition.col + cell->GetWidth() > GameManagerConfig::kBoardWidth ||
                cellPosition.row < 0 || cellPosition.row + cell->GetHeight() > GameManagerConfig::kBoardHeight)
            {
                return false;
            }

            for (std::size_t i = 0; i < cell->GetHeight(); ++i)
            {
                for (std::size_t j = 0; j < cell->GetWidth(); ++j)
                {
                    if (!layeredCells_[cellPosition.row + i][cellPosition.col + j].CanBuild())
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        template <typename TCell, typename... TArgs>
        bool Build(CellPosition cellPosition, TArgs... args)
        {
            auto cell = std::make_shared<TCell>(cellPosition, args...);
            if (!CanBuild(cell))
                return false;

            CellPosition topLeft = cell->GetTopLeftCellPosition();

            for (std::size_t i = 0; i < cell->GetHeight(); ++i)
            {
                for (std::size_t j = 0; j < cell->GetWidth(); ++j)
                {
                    layeredCells_[topLeft.row + i][topLeft.col + j].SetForegrund(cell);
                }
            }
            return true;
        }

        void Remove(CellPosition cellPosition)
        {
            auto foreground = layeredCells_[cellPosition.row][cellPosition.col].GetForeground();

            if (foreground != nullptr)
            {
                if (foreground->CanRemove())
                {
                    auto topLeftCellPosition = foreground->GetTopLeftCellPosition();

                    for (std::size_t i = 0; i < foreground->GetHeight(); ++i)
                    {
                        for (std::size_t j = 0; j < foreground->GetWidth(); ++j)
                        {
                            layeredCells_[topLeftCellPosition.row + i][topLeftCellPosition.col + j].SetForegrund(nullptr);
                        }
                    }
                }
            }
        }

        void SetBackground(CellPosition cellPosition, std::shared_ptr<IBackgroundCell> value)
        {
            layeredCells_[cellPosition.row][cellPosition.col].SetBackground(value);
        }

        void Update()
        {
            for (int row = 0; row < GameManagerConfig::kBoardHeight; ++row)
            {
                for (int col = 0; col < GameManagerConfig::kBoardWidth; ++col)
                {
                    auto &layeredCell = layeredCells_[row][col];
                    auto foreground = layeredCell.GetForeground();
                    if (foreground != nullptr)
                    {
                        foreground->UpdatePassOne({row, col}, *this);
                    }
                }
            }
            for (int row = 0; row < GameManagerConfig::kBoardHeight; ++row)
            {
                for (int col = 0; col < GameManagerConfig::kBoardWidth; ++col)
                {
                    auto &layeredCell = layeredCells_[row][col];
                    auto foreground = layeredCell.GetForeground();
                    if (foreground != nullptr)
                    {
                        foreground->UpdatePassTwo({row, col}, *this);
                    }
                }
            }
        }

    private:
        std::array<std::array<LayeredCell, GameManagerConfig::kBoardWidth>, GameManagerConfig::kBoardHeight> layeredCells_;
    };

    bool IsWithinBoard(CellPosition cellPosition)
    {
        return cellPosition.row >= 0 && cellPosition.row < GameManagerConfig::kBoardHeight &&
               cellPosition.col >= 0 && cellPosition.col < GameManagerConfig::kBoardWidth;
    }

    void SendProduct(GameBoard &board, CellPosition cellPosition, Direction direction, int product)
    {
        CellPosition targetCellPosition = GetNeighborCellPosition(cellPosition, direction);

        if (!IsWithinBoard(targetCellPosition))
            return;

        auto foregroundCell = board.GetLayeredCell(targetCellPosition).GetForeground();

        if (foregroundCell)
        {
            foregroundCell->ReceiveProduct(targetCellPosition, product);
        }
    }

    std::size_t GetNeighborCapacity(const GameBoard &board, CellPosition cellPosition, Direction direction)
    {
        CellPosition neighborCellPosition = GetNeighborCellPosition(cellPosition, direction);

        if (!IsWithinBoard(neighborCellPosition))
            return 0;

        auto foregroundCell = board.GetLayeredCell(neighborCellPosition).GetForeground();

        if (foregroundCell)
        {
            return foregroundCell->GetCapacity(neighborCellPosition);
        }
        return 0;
    }

    class NumberCell : public IBackgroundCell
    {
    public:
        NumberCell(int number) : number_(number) {}

        int GetNumber() const
        {
            return number_;
        }

        bool CanBuild() const override
        {
            return true;
        }

        void Accept(const CellVisitor *visitor) const override
        {
            visitor->Visit(this);
        }

    private:
        int number_;
    };

    class BackgroundCellFactory
    {
    public:
        BackgroundCellFactory(unsigned int seed) : gen_(seed) {}

        std::shared_ptr<IBackgroundCell> Create()
        {
            int val = gen_() % 30;

            std::set<int> numbers = {1, 2, 3, 5, 7, 11};

            if (numbers.count(val))
            {
                return std::make_shared<NumberCell>(val);
            }
            else
            {
                return nullptr;
            }
        }

    private:
        std::mt19937 gen_;
    };

    class MiningMachineCell : public ForegroundCell
    {
    public:
        MiningMachineCell(CellPosition topLeft, Direction direction)
            : ForegroundCell(topLeft), direction_{direction}, elapsedTime_{0} {}

        Direction GetDirection() const { return direction_; }

        void Accept(const CellVisitor *visitor) const override
        {
            visitor->Visit(this);
        }

        bool CanRemove() const override
        {
            return true;
        }
        std::size_t GetCapacity(CellPosition cellPosition) const override
        {
            return 0;
        }
        void ReceiveProduct(CellPosition cellPosition, int number) override
        {
        }
        void UpdatePassOne(CellPosition cellPosition, GameBoard &board) override
        {
            ++elapsedTime_;
            if (elapsedTime_ >= 100)
            {
                auto numberCell =
                    dynamic_cast<const NumberCell *>(board.GetLayeredCell(cellPosition).GetBackground().get());

                if (numberCell && GetNeighborCapacity(board, cellPosition, direction_) >= 3)
                {
                    SendProduct(board, cellPosition, direction_, numberCell->GetNumber());
                }

                elapsedTime_ = 0;
            }
        }

    private:
        Direction direction_;
        std::size_t elapsedTime_;
    };

    enum class PlayerActionType
    {
        None,
        BuildLeftOutMiningMachine,
        BuildTopOutMiningMachine,
        BuildRightOutMiningMachine,
        BuildBottomOutMiningMachine,
        BuildLeftToRightConveyor,
        BuildTopToBottomConveyor,
        BuildRightToLeftConveyor,
        BuildBottomToTopConveyor,
        BuildTopOutCombiner,
        BuildRightOutCombiner,
        BuildBottomOutCombiner,
        BuildLeftOutCombiner,
        Clear,
    };

    struct PlayerAction
    {
        PlayerActionType type;
        CellPosition cellPosition;
    };

    class IGamePlayer
    {
    public:
        virtual PlayerAction GetNextAction(const IGameInfo &info) = 0;
    };

    class GameManager : public IGameManager
    {
    public:
        struct CollectionCenterConfig
        {
            static constexpr int kLeft = GameManagerConfig::kBoardWidth / 2 - GameManagerConfig::kGoalSize / 2;
            static constexpr int kTop = GameManagerConfig::kBoardHeight / 2 - GameManagerConfig::kGoalSize / 2;
        };

        GameManager(
            IGamePlayer *player,
            int commonDividor,
            unsigned int seed)
            : elapsedTime_{}, endTime_{GameManagerConfig::kEndTime}, player_(player), board_(), commonDividor_{commonDividor}, scores_{}
        {
            static_assert(GameManagerConfig::kBoardWidth % 2 == 0, "WIDTH must be even");

            BackgroundCellFactory backgroundCellFactory(seed);

            for (int row = 0; row < GameManagerConfig::kBoardHeight; ++row)
            {
                for (int col = 0; col < GameManagerConfig::kBoardWidth; ++col)
                {
                    auto backgroundCell = backgroundCellFactory.Create();

                    board_.SetBackground({row, col}, backgroundCell);
                }
            };

            auto collectionCenterTopLeftCellPosition =
                CellPosition{CollectionCenterConfig::kTop, CollectionCenterConfig::kLeft};

            board_.template Build<CollectionCenterCell>(collectionCenterTopLeftCellPosition, this);

            std::mt19937 gen(seed);

            for (int k = 1; k <= GameManagerConfig::kNumberOfWalls; ++k)
            {
                CellPosition cellPosition;
                cellPosition.row = gen() % GameManagerConfig::kBoardHeight;
                cellPosition.col = gen() % GameManagerConfig::kBoardWidth;
                if (board_.GetLayeredCell(cellPosition).GetForeground() == nullptr)
                {
                    board_.template Build<WallCell>(cellPosition);
                }
            }
        }

        bool IsGameOver() const override
        {
            return elapsedTime_ >= endTime_;
        }

        int GetEndTime() const override { return endTime_; }

        int GetElapsedTime() const override { return elapsedTime_; }

        std::string GetLevelInfo() const override
        {
            return "(" + std::to_string(commonDividor_) + ")";
        }

        bool IsScoredProduct(int number) const override
        {
            return number % commonDividor_ == 0;
        }

        void OnProductReceived(int number) override
        {
            assert(number != 0);

            if (number % commonDividor_ == 0)
            {
                AddScore();
            }
        }

        int GetScores() const override
        {
            return scores_;
        }

        const LayeredCell &GetLayeredCell(CellPosition cellPosition) const override
        {
            return board_.GetLayeredCell(cellPosition);
        }

        void AddScore()
        {
            scores_++;
        }

        void Update()
        {
            if (elapsedTime_ >= endTime_)
                return;

            ++elapsedTime_;

            if (elapsedTime_ % 3 == 0)
            {
                PlayerAction playerAction = player_->GetNextAction(*this);

                switch (playerAction.type)
                {
                case PlayerActionType::None:
                    break;
                case PlayerActionType::BuildLeftOutMiningMachine:
                    board_.template Build<MiningMachineCell>(playerAction.cellPosition, Direction::kLeft);
                    break;
                case PlayerActionType::BuildTopOutMiningMachine:
                    board_.template Build<MiningMachineCell>(playerAction.cellPosition, Direction::kTop);
                    break;
                case PlayerActionType::BuildRightOutMiningMachine:
                    board_.template Build<MiningMachineCell>(playerAction.cellPosition, Direction::kRight);
                    break;
                case PlayerActionType::BuildBottomOutMiningMachine:
                    board_.template Build<MiningMachineCell>(playerAction.cellPosition, Direction::kBottom);
                    break;
                case PlayerActionType::BuildLeftToRightConveyor:
                    board_.template Build<ConveyorCell>(playerAction.cellPosition, Direction::kRight);
                    break;
                case PlayerActionType::BuildTopToBottomConveyor:
                    board_.template Build<ConveyorCell>(playerAction.cellPosition, Direction::kBottom);
                    break;
                case PlayerActionType::BuildRightToLeftConveyor:
                    board_.template Build<ConveyorCell>(playerAction.cellPosition, Direction::kLeft);
                    break;
                case PlayerActionType::BuildBottomToTopConveyor:
                    board_.template Build<ConveyorCell>(playerAction.cellPosition, Direction::kTop);
                    break;
                case PlayerActionType::BuildTopOutCombiner:
                    board_.template Build<CombinerCell>(playerAction.cellPosition, Direction::kTop);
                    break;
                case PlayerActionType::BuildRightOutCombiner:
                    board_.template Build<CombinerCell>(playerAction.cellPosition, Direction::kRight);
                    break;
                case PlayerActionType::BuildBottomOutCombiner:
                    board_.template Build<CombinerCell>(playerAction.cellPosition, Direction::kBottom);
                    break;
                case PlayerActionType::BuildLeftOutCombiner:
                    board_.template Build<CombinerCell>(playerAction.cellPosition, Direction::kLeft);
                    break;
                case PlayerActionType::Clear:
                    board_.Remove(playerAction.cellPosition);
                    break;
                }
            }

            board_.Update();
        }

    private:
        std::size_t elapsedTime_;
        std::size_t endTime_;
        IGamePlayer *player_;
        GameBoard board_;
        int commonDividor_;
        int scores_;
    };
}
#endif

#ifndef USE_GUI
void Test(int commonDividor, unsigned int seed);

void Test1A() { Test(1, 20); }
void Test1B() { Test(1, 0 /* HIDDEN */); }

void Test2A() { Test(2, 25); }
void Test2B() { Test(2, 0 /* HIDDEN */); }

void Test3A() { Test(3, 30); }
void Test3B() { Test(3, 0 /* HIDDEN */); }

void Test4A() { Test(4, 35); }
void Test4B() { Test(4, 0 /* HIDDEN */); }

void Test5A() { Test(5, 40); }
void Test5B() { Test(5, 0 /* HIDDEN */); }

int main()
{
    int id;
    std::cin >> id;
    void (*f[])() = {Test1A, Test1B, Test2A, Test2B, Test3A, Test3B, Test4A, Test4B, Test5A, Test5B};
    f[id - 1]();
}

class GamePlayer : public Feis::IGamePlayer
{
public:
    Feis::PlayerAction GetNextAction(const Feis::IGameInfo &info) override
    {
        Feis::PlayerAction action;
        action.type = Feis::PlayerActionType::None;

        // 定義目標位置
        Feis::CellPosition targetPosition = {pos_y, pos_x};

        std::string levelStr = info.GetLevelInfo();
        if (!levelStr.empty() && levelStr.front() == '(' && levelStr.back() == ')')
        {
            levelStr = levelStr.substr(1, levelStr.size() - 2);
        }

        int level = std::stoi(levelStr);

        // 確認位置有效
        if (IsValidCellPosition(targetPosition) and (pos_y < Feis::GameManagerConfig::kBoardHeight and pos_x < Feis::GameManagerConfig::kBoardWidth))
        {
            const Feis::LayeredCell &layeredCell = info.GetLayeredCell(targetPosition);
            auto backgroundCell = layeredCell.GetBackground();

            // 在四條邊界上不能使用rightPosition之類的，記得設保護條件
            Feis::CellPosition rightPosition = {pos_y, pos_x + 1};
            Feis::CellPosition leftPosition = {pos_y, pos_x - 1};
            Feis::CellPosition topPosition = {pos_y - 1, pos_x};
            Feis::CellPosition bottomPosition = {pos_y + 1, pos_x};

            // 先檢查邊界條件
            bool isTopValid = pos_y > 0;
            bool isBottomValid = pos_y < Feis::GameManagerConfig::kBoardHeight - 1;
            bool isLeftValid = pos_x > 0;
            bool isRightValid = pos_x < Feis::GameManagerConfig::kBoardWidth - 1;

            // 初始化指向相邻单元格的指针
            const Feis::NumberCell *numberCell_right = nullptr;
            const Feis::NumberCell *numberCell_left = nullptr;
            const Feis::NumberCell *numberCell_top = nullptr;
            const Feis::NumberCell *numberCell_bottom = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_right = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_left = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_top = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_bottom = nullptr;

            if (isRightValid)
            {
                const Feis::LayeredCell &layeredCell_right = info.GetLayeredCell(rightPosition);
                auto backgroundCell_right = layeredCell_right.GetBackground();
                numberCell_right = dynamic_cast<const Feis::NumberCell *>(backgroundCell_right.get());
                foregroundCell_right = layeredCell_right.GetForeground();
            }

            if (isLeftValid)
            {
                const Feis::LayeredCell &layeredCell_left = info.GetLayeredCell(leftPosition);
                auto backgroundCell_left = layeredCell_left.GetBackground();
                numberCell_left = dynamic_cast<const Feis::NumberCell *>(backgroundCell_left.get());
                foregroundCell_left = layeredCell_left.GetForeground();
            }

            if (isTopValid)
            {
                const Feis::LayeredCell &layeredCell_top = info.GetLayeredCell(topPosition);
                auto backgroundCell_top = layeredCell_top.GetBackground();
                numberCell_top = dynamic_cast<const Feis::NumberCell *>(backgroundCell_top.get());
                foregroundCell_top = layeredCell_top.GetForeground();
            }

            if (isBottomValid)
            {
                const Feis::LayeredCell &layeredCell_bottom = info.GetLayeredCell(bottomPosition);
                auto backgroundCell_bottom = layeredCell_bottom.GetBackground();
                numberCell_bottom = dynamic_cast<const Feis::NumberCell *>(backgroundCell_bottom.get());
                foregroundCell_bottom = layeredCell_bottom.GetForeground();
            }

            // foregroundCell 非空格的意思

            if (backgroundCell != nullptr)
            {
                // 如果是數字格
                const Feis::NumberCell *numberCell = dynamic_cast<const Feis::NumberCell *>(backgroundCell.get());
                // Part(1)處理數字格
                if (numberCell != nullptr)
                {
                    int number = numberCell->GetNumber();

                    int number_left = 0;
                    if (numberCell_left != nullptr)
                        number_left = numberCell_left->GetNumber();

                    int number_right = 0;
                    if (numberCell_right != nullptr)
                        number_right = numberCell_right->GetNumber();

                    int number_top = 0;
                    if (numberCell_top != nullptr)
                        number_top = numberCell_top->GetNumber();

                    int number_bottom = 0;
                    if (numberCell_bottom != nullptr)
                        number_bottom = numberCell_bottom->GetNumber();

                    // 撲滿十字輸送帶
                    // bottom to top

                    if (pos_x >= 29 && pos_x <= 32 && pos_y > 19)
                    {
                        if (pos_y == 20)
                            ActivateConveyorFromBottomToTop(action, targetPosition);

                        else if (pos_y == 26)
                        {
                            if (pos_x == 29)
                            {
                                if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false)
                                    ActivateConveyorFromBottomToTop(action, targetPosition);

                                else
                                    ActivateConveyorFromLeftToRight(action, targetPosition);
                            }

                            else if (pos_x == 32)
                            {
                                if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false)
                                    ActivateConveyorFromBottomToTop(action, targetPosition);

                                else
                                    ActivateConveyorFromRightToLeft(action, targetPosition);
                            }

                            else
                                ActivateConveyorFromBottomToTop(action, targetPosition);
                        }

                        else if (foregroundCell_top->CanRemove() == false and (pos_x == 29 or pos_x == 30))
                            ActivateConveyorFromLeftToRight(action, targetPosition);

                        else if (foregroundCell_top->CanRemove() == false and (pos_x == 31 or pos_x == 32))
                            ActivateConveyorFromRightToLeft(action, targetPosition);

                        else
                            ActivateConveyorFromBottomToTop(action, targetPosition);
                    }

                    // left to right
                    else if (pos_x < 29 and pos_y >= 16 and pos_y <= 19)
                    {
                        if (pos_x == 28)
                            ActivateConveyorFromLeftToRight(action, targetPosition);

                        else if (pos_x == 15)
                        {
                            if (pos_y == 16)
                            {
                                if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false)
                                    ActivateConveyorFromLeftToRight(action, targetPosition);

                                else
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }

                            else if (pos_y == 19)
                            {
                                if (foregroundCell_top != nullptr and foregroundCell_top->CanRemove() == false)
                                    ActivateConveyorFromLeftToRight(action, targetPosition);

                                else
                                    ActivateConveyorFromBottomToTop(action, targetPosition);
                            }

                            else
                                ActivateConveyorFromLeftToRight(action, targetPosition);
                        }

                        else if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false and (pos_y == 16 or pos_y == 17))
                            ActivateConveyorFromTopToBottom(action, targetPosition);

                        else if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false and (pos_y == 18 or pos_y == 19))
                            ActivateConveyorFromBottomToTop(action, targetPosition);

                        else
                            ActivateConveyorFromLeftToRight(action, targetPosition);
                    }

                    // 上到下
                    else if (pos_x >= 29 && pos_x <= 32 && pos_y < 16)
                    {
                        if (pos_y == 15)
                            ActivateConveyorFromTopToBottom(action, targetPosition);

                        else if (pos_y == 7)
                        {
                            if (pos_x == 29)
                            {
                                if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);

                                else
                                    ActivateConveyorFromLeftToRight(action, targetPosition);
                            }

                            else if (pos_x == 32)
                            {
                                if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);

                                else
                                    ActivateConveyorFromRightToLeft(action, targetPosition);
                            }

                            else
                                ActivateConveyorFromTopToBottom(action, targetPosition);
                        }

                        else if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false and (pos_x == 29 or pos_x == 30))
                            ActivateConveyorFromLeftToRight(action, targetPosition);

                        else if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false and (pos_x == 31 or pos_x == 32))
                            ActivateConveyorFromRightToLeft(action, targetPosition);

                        else
                            ActivateConveyorFromTopToBottom(action, targetPosition);
                    }

                    // right to left
                    else if (pos_x > 32 and pos_y >= 16 and pos_y <= 19)
                    {
                        if (pos_x == 33)
                            ActivateConveyorFromRightToLeft(action, targetPosition);

                        else if (pos_x == 47)
                        {
                            if (pos_y == 16)
                            {
                                if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false)
                                    ActivateConveyorFromRightToLeft(action, targetPosition);

                                else
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }

                            else if (pos_y == 19)
                            {
                                if (foregroundCell_top != nullptr and foregroundCell_top->CanRemove() == false)
                                    ActivateConveyorFromRightToLeft(action, targetPosition);

                                else
                                    ActivateConveyorFromBottomToTop(action, targetPosition);
                            }

                            else
                                ActivateConveyorFromRightToLeft(action, targetPosition);
                        }

                        else if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false and (pos_y == 16 or pos_y == 17))
                            ActivateConveyorFromTopToBottom(action, targetPosition);

                        else if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false and (pos_y == 18 or pos_y == 19))
                            ActivateConveyorFromBottomToTop(action, targetPosition);

                        else
                            ActivateConveyorFromRightToLeft(action, targetPosition);
                    }

                    // commomdividor == X 時
                    // level = 2 3 5 執行動作都一樣，level == 4還沒寫

                    // 目前僅能處理兩個方塊卡住輸送帶
                    // 因為是從左上到右下蓋東西
                    // 所以在左上遇到要轉彎的情況，最單純僅考慮下方或右方是否為牆壁或礦坑

                    // 在下方或右方，須考慮上或左（已經放東西的點），是礦坑還是牆壁（不能用foregroundCell_top->CanRemove()之類的判斷）
                    // 一定要用(numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove())這種條件判斷
                    // 不然可能遇到左邊為蓋好的輸送帶，卻認為要轉彎
                    // 只有啟動的礦坑或牆壁會擋路

                    // 使用foregroundCell_top->CanRemove() number_top == level 等記得設保護條件
                    // 先確保兩者不是nullptr後再使用，不然會RE
                    // E.G.(foregroundCell_top != nullptr && !foregroundCell_top->CanRemove())

                    // 解釋：numberCell_top，數字點的上方格子
                    //      number_top，數字點上方格子的數字（如果有的話）
                    //      number_top，數字點上方格子的數字（如果有的話）
                    //      foregroundCell != nullptr，該點有蓋東西（礦坑、輸送帶、或是障礙等等）
                    //      foregroundCell -> CanRemove該點有蓋東西且可以拆掉（礦坑、輸送）（障礙不能拆！！）
                    //      剩下依此類推，想不到再問我

                    else if (level == 2)
                    {

                        // 啟動對應的礦坑
                        // 透過轉換啟動方向確保能輸出
                        if (number == 2)
                        {
                            if (pos_x > 32)
                            {
                                if ((numberCell_left != nullptr && number_left == level) || (foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()))
                                {
                                    if ((numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove()))
                                    {

                                        if ((numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()))
                                            ActivateMiningMachineToRight(action, targetPosition);

                                        else
                                            ActivateMiningMachineToBottom(action, targetPosition);
                                    }

                                    else
                                        ActivateMiningMachineToTop(action, targetPosition);
                                }

                                else
                                    ActivateMiningMachineToLeft(action, targetPosition);
                            }

                            else if (pos_x < 29)
                            {
                                if ((numberCell_right != nullptr && number_right == level) || (foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()))
                                {
                                    if ((numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()))
                                    {

                                        if ((numberCell_left != nullptr && number_left == level) || (foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()))
                                            ActivateMiningMachineToTop(action, targetPosition);

                                        else
                                            ActivateMiningMachineToLeft(action, targetPosition);
                                    }

                                    else
                                        ActivateMiningMachineToBottom(action, targetPosition);
                                }

                                else
                                    ActivateMiningMachineToRight(action, targetPosition);
                            }
                        }

                        // 在數字點蓋輸送帶（Commomdividor != 1 的情況下）
                        else
                        {
                            // 右半部
                            if (pos_x > 32)
                            {
                                if ((numberCell_left != nullptr && number_left == level) || (foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()))
                                {
                                    if ((numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove()))
                                    {
                                        if (pos_y >= 19)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);
                                        else if (pos_y <= 18)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);
                                    }

                                    else if ((foregroundCell_bottom != nullptr and !foregroundCell_bottom->CanRemove()) || (numberCell_bottom != nullptr and number_bottom == level))
                                    {
                                        if (pos_y <= 18)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);

                                        else if (pos_y >= 19)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);
                                    }

                                    else if (pos_y <= 18)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);

                                    else if (pos_y >= 19)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }

                                else
                                {
                                    ActivateConveyorFromRightToLeft(action, targetPosition);
                                }
                            }

                            // 左半部
                            else if (pos_x < 29)
                            {

                                if ((numberCell_right != nullptr and number_right == level) || (foregroundCell_right != nullptr and !foregroundCell_right->CanRemove()))
                                {
                                    if (foregroundCell_top != nullptr && ((numberCell_top != nullptr and number_top == level) || (!foregroundCell_top->CanRemove()) and foregroundCell_top != nullptr))
                                    {

                                        if (pos_y >= 19)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);

                                        else if (pos_y <= 18)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);
                                    }

                                    else if (foregroundCell_bottom != nullptr && ((numberCell_bottom != nullptr and number_bottom == number) || (!foregroundCell_bottom->CanRemove()) and foregroundCell_bottom != nullptr))
                                    {

                                        if (pos_y <= 18)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);

                                        else if (pos_y >= 19)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);
                                    }

                                    else if (pos_y <= 18)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);

                                    else if (pos_y >= 19)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }
                                else
                                {
                                    ActivateConveyorFromLeftToRight(action, targetPosition);
                                }
                            }
                        }
                    }

                    else if (level == 3)
                    {

                        if (number == 3)
                        {
                            if (pos_x > 32)
                            {
                                if ((numberCell_left != nullptr && number_left == level) || (foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()))
                                {
                                    if ((numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove()))
                                    {

                                        if ((numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()))
                                            ActivateMiningMachineToRight(action, targetPosition);

                                        else
                                            ActivateMiningMachineToBottom(action, targetPosition);
                                    }

                                    else
                                        ActivateMiningMachineToTop(action, targetPosition);
                                }

                                else
                                    ActivateMiningMachineToLeft(action, targetPosition);
                            }

                            else if (pos_x < 29)
                            {
                                if ((numberCell_right != nullptr && number_right == level) || (foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()))
                                {
                                    if ((numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()))
                                    {

                                        if ((numberCell_left != nullptr && number_left == level) || (foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()))
                                            ActivateMiningMachineToTop(action, targetPosition);

                                        else
                                            ActivateMiningMachineToLeft(action, targetPosition);
                                    }

                                    else
                                        ActivateMiningMachineToBottom(action, targetPosition);
                                }

                                else
                                    ActivateMiningMachineToRight(action, targetPosition);
                            }
                        }

                        // 在數字點蓋輸送帶（Commomdividor != 1 的情況下）
                        else
                        {
                            // 右半部
                            if (pos_x > 32)
                            {
                                if ((numberCell_left != nullptr && number_left == level) || (foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()))
                                {
                                    if ((numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove()))
                                    {
                                        if (pos_y >= 19)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);
                                        else if (pos_y <= 18)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);
                                    }

                                    else if ((foregroundCell_bottom != nullptr and !foregroundCell_bottom->CanRemove()) || (numberCell_bottom != nullptr and number_bottom == level))
                                    {
                                        if (pos_y <= 18)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);

                                        else if (pos_y >= 19)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);
                                    }

                                    else if (pos_y <= 18)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);

                                    else if (pos_y >= 19)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }

                                else
                                {
                                    ActivateConveyorFromRightToLeft(action, targetPosition);
                                }
                            }

                            // 左半部
                            else if (pos_x < 29)
                            {

                                if ((numberCell_right != nullptr and number_right == level) || (foregroundCell_right != nullptr and !foregroundCell_right->CanRemove()))
                                {
                                    if (foregroundCell_top != nullptr && ((numberCell_top != nullptr and number_top == level) || (!foregroundCell_top->CanRemove()) and foregroundCell_top != nullptr))
                                    {

                                        if (pos_y >= 19)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);

                                        else if (pos_y <= 18)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);
                                    }

                                    else if (foregroundCell_bottom != nullptr && ((numberCell_bottom != nullptr and number_bottom == number) || (!foregroundCell_bottom->CanRemove()) and foregroundCell_bottom != nullptr))
                                    {

                                        if (pos_y <= 18)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);

                                        else if (pos_y >= 19)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);
                                    }

                                    else if (pos_y <= 18)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);

                                    else if (pos_y >= 19)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }
                                else
                                {
                                    ActivateConveyorFromLeftToRight(action, targetPosition);
                                }
                            }
                        }
                    }

                    else if (level == 5)
                    {

                        if (number == 5)
                        {
                            if (pos_x > 32)
                            {
                                if ((numberCell_left != nullptr && number_left == level) || (foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()))
                                {
                                    if ((numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove()))
                                    {

                                        if ((numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()))
                                            ActivateMiningMachineToRight(action, targetPosition);

                                        else
                                            ActivateMiningMachineToBottom(action, targetPosition);
                                    }

                                    else
                                        ActivateMiningMachineToTop(action, targetPosition);
                                }

                                else
                                    ActivateMiningMachineToLeft(action, targetPosition);
                            }

                            else if (pos_x < 29)
                            {
                                if ((numberCell_right != nullptr && number_right == level) || (foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()))
                                {
                                    if ((numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()))
                                    {

                                        if ((numberCell_left != nullptr && number_left == level) || (foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()))
                                            ActivateMiningMachineToTop(action, targetPosition);

                                        else
                                            ActivateMiningMachineToLeft(action, targetPosition);
                                    }

                                    else
                                        ActivateMiningMachineToBottom(action, targetPosition);
                                }

                                else
                                    ActivateMiningMachineToRight(action, targetPosition);
                            }
                        }

                        // 在數字點蓋輸送帶（Commomdividor != 1 的情況下）
                        else
                        {
                            // 右半部
                            if (pos_x > 32)
                            {
                                if ((numberCell_left != nullptr && number_left == level) || (foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()))
                                {
                                    if ((numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove()))
                                    {
                                        if (pos_y >= 19)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);
                                        else if (pos_y <= 18)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);
                                    }

                                    else if ((foregroundCell_bottom != nullptr and !foregroundCell_bottom->CanRemove()) || (numberCell_bottom != nullptr and number_bottom == level))
                                    {
                                        if (pos_y <= 18)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);

                                        else if (pos_y >= 19)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);
                                    }

                                    else if (pos_y <= 18)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);

                                    else if (pos_y >= 19)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }

                                else
                                {
                                    ActivateConveyorFromRightToLeft(action, targetPosition);
                                }
                            }

                            // 左半部
                            else if (pos_x < 29)
                            {

                                if ((numberCell_right != nullptr and number_right == level) || (foregroundCell_right != nullptr and !foregroundCell_right->CanRemove()))
                                {
                                    if (foregroundCell_top != nullptr && ((numberCell_top != nullptr and number_top == level) || (!foregroundCell_top->CanRemove()) and foregroundCell_top != nullptr))
                                    {

                                        if (pos_y >= 19)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);

                                        else if (pos_y <= 18)
                                            ActivateConveyorFromTopToBottom(action, targetPosition);
                                    }

                                    else if (foregroundCell_bottom != nullptr && ((numberCell_bottom != nullptr and number_bottom == number) || (!foregroundCell_bottom->CanRemove()) and foregroundCell_bottom != nullptr))
                                    {

                                        if (pos_y <= 18)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);

                                        else if (pos_y >= 19)
                                            ActivateConveyorFromBottomToTop(action, targetPosition);
                                    }

                                    else if (pos_y <= 18)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);

                                    else if (pos_y >= 19)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }
                                else
                                {
                                    ActivateConveyorFromLeftToRight(action, targetPosition);
                                }
                            }
                        }
                    }
                    // 如果commomDividor是1
                    // 不知道為何加上牆壁的判斷分數更低，索性不加
                    else
                    {
                        if (pos_x < 29)
                        {
                            if (numberCell_right != nullptr)
                            {
                                if (numberCell_top != nullptr)
                                {

                                    if (numberCell_left != nullptr)
                                        ActivateMiningMachineToBottom(action, targetPosition);

                                    else
                                        ActivateMiningMachineToLeft(action, targetPosition);
                                }

                                else
                                    ActivateMiningMachineToTop(action, targetPosition);
                            }

                            else
                                ActivateMiningMachineToRight(action, targetPosition);
                        }

                        else if (pos_x > 32)
                        {
                            if (numberCell_left != nullptr)
                            {
                                if (numberCell_right != nullptr)
                                {

                                    if (numberCell_bottom != nullptr)
                                        ActivateMiningMachineToTop(action, targetPosition);

                                    else
                                        ActivateMiningMachineToBottom(action, targetPosition);
                                }

                                else
                                    ActivateMiningMachineToRight(action, targetPosition);
                            }

                            else
                                ActivateMiningMachineToLeft(action, targetPosition);
                        }
                    }
                }
            }

            /*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
            /*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

            // Part(2)處理非數字格
            else
            {
                // 在十字區域蓋上輸送帶

                // 下到上
                if (pos_x >= 29 && pos_x <= 32 && pos_y > 19)
                {
                    if (pos_y == 20)
                        ActivateConveyorFromBottomToTop(action, targetPosition);

                    else if (pos_y == 26)
                    {
                        if (pos_x == 29)
                        {
                            if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false)
                                ActivateConveyorFromBottomToTop(action, targetPosition);

                            else
                                ActivateConveyorFromLeftToRight(action, targetPosition);
                        }

                        else if (pos_x == 32)
                        {
                            if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false)
                                ActivateConveyorFromBottomToTop(action, targetPosition);

                            else
                                ActivateConveyorFromRightToLeft(action, targetPosition);
                        }

                        else
                            ActivateConveyorFromBottomToTop(action, targetPosition);
                    }

                    else if (foregroundCell_top != nullptr and foregroundCell_top->CanRemove() == false and (pos_x == 29 or pos_x == 30))
                        ActivateConveyorFromLeftToRight(action, targetPosition);

                    else if (foregroundCell_top != nullptr and foregroundCell_top->CanRemove() == false and (pos_x == 31 or pos_x == 32))
                        ActivateConveyorFromRightToLeft(action, targetPosition);

                    else
                        ActivateConveyorFromBottomToTop(action, targetPosition);
                }

                // 左到右
                else if (pos_x < 29 and pos_y >= 16 and pos_y <= 19)
                {
                    if (pos_x == 28)
                        ActivateConveyorFromLeftToRight(action, targetPosition);

                    else if (pos_x == 15)
                    {
                        if (pos_y == 16)
                        {
                            if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false)
                                ActivateConveyorFromLeftToRight(action, targetPosition);

                            else
                                ActivateConveyorFromTopToBottom(action, targetPosition);
                        }

                        else if (pos_y == 19)
                        {
                            if (foregroundCell_top != nullptr and foregroundCell_top->CanRemove() == false)
                                ActivateConveyorFromLeftToRight(action, targetPosition);

                            else
                                ActivateConveyorFromBottomToTop(action, targetPosition);
                        }

                        else
                            ActivateConveyorFromLeftToRight(action, targetPosition);
                    }

                    else if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false and (pos_y == 16 or pos_y == 17))
                        ActivateConveyorFromTopToBottom(action, targetPosition);

                    else if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false and (pos_y == 18 or pos_y == 19))
                        ActivateConveyorFromBottomToTop(action, targetPosition);

                    else
                        ActivateConveyorFromLeftToRight(action, targetPosition);
                }

                // 上到下
                else if (pos_x >= 29 && pos_x <= 32 && pos_y < 16)
                {

                    if (pos_y == 15)
                        ActivateConveyorFromTopToBottom(action, targetPosition);

                    else if (pos_y == 7)
                    {
                        if (pos_x == 29)
                        {
                            if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false)
                                ActivateConveyorFromTopToBottom(action, targetPosition);

                            else
                                ActivateConveyorFromLeftToRight(action, targetPosition);
                        }

                        else if (pos_x == 32)
                        {
                            if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false)
                                ActivateConveyorFromTopToBottom(action, targetPosition);

                            else
                                ActivateConveyorFromRightToLeft(action, targetPosition);
                        }

                        else
                            ActivateConveyorFromTopToBottom(action, targetPosition);
                    }

                    else if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false and (pos_x == 29 or pos_x == 30))
                        ActivateConveyorFromLeftToRight(action, targetPosition);

                    else if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false and (pos_x == 31 or pos_x == 32))
                        ActivateConveyorFromRightToLeft(action, targetPosition);

                    else
                        ActivateConveyorFromTopToBottom(action, targetPosition);
                }

                // 右到左
                else if (pos_x > 32 and pos_y >= 16 and pos_y <= 19)

                {
                    if (pos_x == 33)
                        ActivateConveyorFromRightToLeft(action, targetPosition);

                    else if (pos_x == 47)
                    {
                        if (pos_y == 16)
                        {
                            if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false)
                                ActivateConveyorFromRightToLeft(action, targetPosition);

                            else
                                ActivateConveyorFromTopToBottom(action, targetPosition);
                        }

                        else if (pos_y == 19)
                        {
                            if (foregroundCell_top != nullptr and foregroundCell_top->CanRemove() == false)
                                ActivateConveyorFromRightToLeft(action, targetPosition);

                            else
                                ActivateConveyorFromBottomToTop(action, targetPosition);
                        }

                        else
                            ActivateConveyorFromRightToLeft(action, targetPosition);
                    }

                    else if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false and (pos_y == 16 or pos_y == 17))
                        ActivateConveyorFromTopToBottom(action, targetPosition);

                    else if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false and (pos_y == 18 or pos_y == 19))
                        ActivateConveyorFromBottomToTop(action, targetPosition);

                    else
                        ActivateConveyorFromRightToLeft(action, targetPosition);
                }

                // 右側輸送帶蓋法
                else if (pos_x > 33)
                {
                    // Commdividor如果是2 3 5
                    // 只啟動2 3 5 的礦
                    // 在輸送帶方向判定需要更新
                    // 如果可能會擋路的數字點有啟動，才要轉彎
                    // 啟動的數字點為2 3 5

                    int number_left = 0;
                    if (numberCell_left != nullptr)
                        number_left = numberCell_left->GetNumber();

                    int number_top = 0;
                    if (numberCell_top != nullptr)
                        number_top = numberCell_top->GetNumber();

                    int number_bottom = 0;
                    if (numberCell_bottom != nullptr)
                        number_bottom = numberCell_bottom->GetNumber();

                    // Commdividor如果是1則啟動所有礦
                    if (level == 1)
                    {

                        if (abs(pos_y - 19) < abs(pos_x - 32))
                        {
                            // zone 4
                            if (pos_y <= 18)
                            {
                                if ((foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()) or numberCell_bottom != nullptr)
                                {
                                    if ((foregroundCell_left != nullptr and !foregroundCell_left->CanRemove()) || numberCell_left != nullptr)
                                    {
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                    }

                                    else
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                }

                                else
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }

                            // zone 7
                            else if (pos_y >= 19)
                            {
                                if (foregroundCell_top != nullptr && ((numberCell_top != nullptr) || !foregroundCell_top->CanRemove()))
                                {
                                    if ((foregroundCell_left != nullptr and !foregroundCell_left->CanRemove()) || numberCell_left != nullptr)
                                    {
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    }

                                    else
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                }

                                else
                                    ActivateConveyorFromBottomToTop(action, targetPosition);
                            }

                            ////
                        }
                        // pos_y >= 19 zone 8
                        // pos_y >= 19 zone 4
                        if (abs(pos_y - 19) >= abs(pos_x - 32))
                        {
                            if ((foregroundCell_left != nullptr and !foregroundCell_left->CanRemove()) || numberCell_left != nullptr)
                            {
                                if (foregroundCell_top != nullptr && ((numberCell_top != nullptr) || !foregroundCell_top->CanRemove()))
                                {

                                    if (pos_y >= 19)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);

                                    else if (pos_y <= 18)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                }

                                else if ((foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()) or numberCell_bottom != nullptr)
                                {

                                    ActivateConveyorFromLeftToRight(action, targetPosition);
                                }

                                else if (pos_y <= 18)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);

                                else if (pos_y >= 19)
                                    ActivateConveyorFromBottomToTop(action, targetPosition);
                            }

                            else
                            {
                                ActivateConveyorFromRightToLeft(action, targetPosition);
                            }
                        }
                    }

                    else
                    {
                        // commondividor == 2 || 3 || 5
                        if ((foregroundCell_left != nullptr && !foregroundCell_left->CanRemove()) or (numberCell_left != nullptr && number_left == level))
                        {
                            if ((foregroundCell_top != nullptr && !foregroundCell_top->CanRemove()) or (numberCell_top != nullptr && number_top == level))
                            {
                                if (pos_y >= 19)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);

                                else if (pos_y <= 18)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }

                            else if ((foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()) or (numberCell_bottom != nullptr && number_bottom == level))
                            {
                                if (pos_y <= 18)
                                    ActivateConveyorFromBottomToTop(action, targetPosition);

                                else if (pos_y >= 19)
                                    ActivateConveyorFromBottomToTop(action, targetPosition);
                            }

                            else if (pos_y <= 18)
                                ActivateConveyorFromTopToBottom(action, targetPosition);

                            else if (pos_y >= 19)
                                ActivateConveyorFromBottomToTop(action, targetPosition);
                        }

                        else
                        {
                            ActivateConveyorFromRightToLeft(action, targetPosition);
                        }
                    }
                }

                // 不是礦點的話，這兩點要確保能推進去，因為中心的集中場會被當成障礙物
                // 也許可以修正但我覺得沒有必要
                else if (pos_x == 28)
                {

                    if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false)
                        ActivateConveyorFromTopToBottom(action, targetPosition);

                    else
                        ActivateConveyorFromLeftToRight(action, targetPosition);
                }

                else if (pos_x == 33)
                {
                    if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false)
                        ActivateConveyorFromTopToBottom(action, targetPosition);

                    else
                        ActivateConveyorFromRightToLeft(action, targetPosition);
                }

                // 左側輸送帶蓋法，邏輯一樣
                else if (pos_x <= 27)
                {
                    int number_right = 0;
                    if (numberCell_right != nullptr)
                        number_right = numberCell_right->GetNumber();

                    int number_top = 0;
                    if (numberCell_top != nullptr)
                        number_top = numberCell_top->GetNumber();

                    int number_bottom = 0;
                    if (numberCell_bottom != nullptr)
                        number_bottom = numberCell_bottom->GetNumber();

                    if (level == 1)
                    {
                        // if(16 - pos_y > 29 - pos_x)

                        if (abs(16 - pos_y) < abs(29 - pos_x))
                        {
                            if (pos_y <= 18)
                            {
                                if ((foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()) or numberCell_bottom != nullptr)
                                {
                                    if ((foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()) || numberCell_right != nullptr)
                                    {
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                    }

                                    else
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                }

                                else
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }

                            else if (pos_y >= 19)
                            {
                                if (foregroundCell_top != nullptr && ((numberCell_top != nullptr) || !foregroundCell_top->CanRemove()))
                                {
                                    if ((numberCell_right != nullptr) || (foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()))
                                    {
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                    }

                                    else
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                }

                                else
                                    ActivateConveyorFromBottomToTop(action, targetPosition);
                            }

                            ////
                        }

                        if (abs(16 - pos_y) >= abs(29 - pos_x))
                        {
                            if ((numberCell_right != nullptr) || (foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()))
                            {
                                if (foregroundCell_top != nullptr && ((numberCell_top != nullptr) || !foregroundCell_top->CanRemove()))
                                {

                                    if (pos_y >= 19)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);

                                    else if (pos_y <= 18)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                }

                                else if (foregroundCell_bottom != nullptr && ((numberCell_bottom != nullptr) || !foregroundCell_bottom->CanRemove()))
                                {
                                    ActivateConveyorFromRightToLeft(action, targetPosition);
                                }

                                else if (pos_y <= 18)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);

                                else if (pos_y >= 19)
                                    ActivateConveyorFromBottomToTop(action, targetPosition);
                            }

                            else
                            {
                                ActivateConveyorFromLeftToRight(action, targetPosition);
                            }
                        }
                    }

                    else
                    {
                        if ((numberCell_right != nullptr and number_right == level) || (foregroundCell_right != nullptr and !foregroundCell_right->CanRemove()))
                        {
                            if ((foregroundCell_top != nullptr && !foregroundCell_top->CanRemove()) or ((numberCell_top != nullptr and number_top == level)))
                            {
                                if (pos_y >= 19)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);

                                else if (pos_y <= 18)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }

                            else if ((foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove()) or ((numberCell_bottom != nullptr and number_bottom == level)))
                            {
                                if (pos_y <= 18)
                                    ActivateConveyorFromBottomToTop(action, targetPosition);

                                else if (pos_y >= 19)
                                    ActivateConveyorFromBottomToTop(action, targetPosition);
                            }

                            else if (pos_y <= 18)
                                ActivateConveyorFromTopToBottom(action, targetPosition);

                            else if (pos_y >= 19)
                                ActivateConveyorFromBottomToTop(action, targetPosition);
                        }

                        else
                        {
                            ActivateConveyorFromLeftToRight(action, targetPosition);
                        }
                    }
                }
            }

            // 移動到下一個單元格
            // 從左上到右下

            if (runByTopRows == 1)
            {
                pos_x++;
                times_top++;

                if (times_top == 4 + 2 * (rounds - 1))
                {

                    runByBottomRows = 1;
                    runByTopRows = 0;
                    times_top = 0;

                    pos_x -= 4 + 2 * (rounds - 1);
                    pos_y += 5 + 2 * (rounds - 1);
                }
            }

            else if (runByBottomRows == 1)
            {
                pos_x++;
                times_bottom++;

                if (times_bottom == 4 + 2 * (rounds - 1))
                {
                    times_bottom = 0;
                    runByBottomRows = 0;
                    runByLeftColumns = 1;

                    pos_x -= 5 + 2 * (rounds - 1);

                    pos_y -= 5 + 2 * (rounds - 1);
                }
            }

            else if (runByLeftColumns == 1)
            {
                pos_y++;
                times_left++;

                if (times_left == 6 + 2 * (rounds - 1))
                {
                    times_left = 0;
                    runByLeftColumns = 0;
                    runByRightColumns = 1;

                    pos_x += 5 + 2 * (rounds - 1);
                    pos_y -= 6 + 2 * (rounds - 1);
                }
            }

            else if (runByRightColumns == 1)
            {
                pos_y++;
                times_right++;

                if (times_right == 6 + 2 * (rounds - 1))
                {
                    times_right = 0;
                    runByRightColumns = 0;
                    runByTopRows = 1;

                    pos_x -= 5 + 2 * (rounds - 1);
                    pos_y -= 7 + 2 * (rounds - 1);

                    rounds++;
                    if (rounds == 17)
                    {
                        pos_x = 12;
                        pos_y = 0;

                        runByTopRows = 0;
                        runByLeftColumns = 0;
                        runByRightColumns = 0;
                        runByBottomRows = 0;
                    }
                }
            }

            else
            {

                if (runByLeftColumnsLast == 1)
                {
                    pos_y++;
                    if (pos_y >= Feis::GameManagerConfig::kBoardHeight)
                    {

                        pos_x = 48 + rounds_2;
                        pos_y = 0;

                        runByLeftColumnsLast = 0;
                        runByRightColumnsLast = 1;
                    }
                }

                else if (runByRightColumnsLast == 1)
                {

                    pos_y++;
                    if (pos_y >= Feis::GameManagerConfig::kBoardHeight)
                    {
                        pos_x = 12 - rounds_2;
                        pos_y = 0;

                        runByLeftColumnsLast = 1;
                        runByRightColumnsLast = 0;

                        rounds++;
                        rounds_2++;
                    }
                }
            }
        }

        return action;
    }

private:
    int pos_x = 29;
    int pos_y = 15;
    int part = 0;

    int rounds = 1;
    int rounds_2 = 1;
    bool firstTime = 1;
    bool firstTimeLast = 1;

    int times_top = 0;
    int times_bottom = 0;
    int times_right = 0;
    int times_left = 0;

    bool runByTopRows = 1;
    bool runByBottomRows = 0;

    bool runByRightColumns = 0;
    bool runByLeftColumns = 0;

    bool runByRightColumnsLast = 0;
    bool runByLeftColumnsLast = 1;

    int conveyorType = 24;

    // x == 29,32 y == 16,19 長方形的邊

    bool IsValidCellPosition(const Feis::CellPosition &position) const
    {
        return position.row >= 0 && position.row < Feis::GameManagerConfig::kBoardHeight &&
               position.col >= 0 && position.col < Feis::GameManagerConfig::kBoardWidth;
    }

    void ActivateMiningMachineToRight(Feis::PlayerAction &action, const Feis::CellPosition &position)
    {
        action.type = Feis::PlayerActionType::BuildRightOutMiningMachine; // 在右邊生成礦坑
        action.cellPosition = position;
    }

    void ActivateMiningMachineToLeft(Feis::PlayerAction &action, const Feis::CellPosition &position)
    {
        action.type = Feis::PlayerActionType::BuildLeftOutMiningMachine; // 在右邊生成礦坑
        action.cellPosition = position;
    }

    void ActivateMiningMachineToTop(Feis::PlayerAction &action, const Feis::CellPosition &position)
    {
        action.type = Feis::PlayerActionType::BuildTopOutMiningMachine; // 在右邊生成礦坑
        action.cellPosition = position;
    }

    void ActivateMiningMachineToBottom(Feis::PlayerAction &action, const Feis::CellPosition &position)
    {
        action.type = Feis::PlayerActionType::BuildBottomOutMiningMachine; // 在右邊生成礦坑
        action.cellPosition = position;
    }

    // 激活輸送帶，從左到右
    void ActivateConveyorFromLeftToRight(Feis::PlayerAction &action, const Feis::CellPosition &position)
    {
        action.type = Feis::PlayerActionType::BuildLeftToRightConveyor; // 生成從左到右的輸送帶
        action.cellPosition = position;
    }

    // 激活輸送帶，從右到左
    void ActivateConveyorFromRightToLeft(Feis::PlayerAction &action, const Feis::CellPosition &position)
    {
        action.type = Feis::PlayerActionType::BuildRightToLeftConveyor; // 生成從右到左的輸送帶
        action.cellPosition = position;
    }

    void ActivateConveyorFromTopToBottom(Feis::PlayerAction &action, const Feis::CellPosition &position)
    {
        action.type = Feis::PlayerActionType::BuildTopToBottomConveyor; // 生成從右到左的輸送帶
        action.cellPosition = position;
    }

    void ActivateConveyorFromBottomToTop(Feis::PlayerAction &action, const Feis::CellPosition &position)
    {
        action.type = Feis::PlayerActionType::BuildBottomToTopConveyor; // 生成從右到左的輸送帶
        action.cellPosition = position;
    }
};

void Test(int commonDividor, unsigned int seed)
{
    GamePlayer player;
    Feis::GameManager gameManager(&player, commonDividor, seed);

    while (!gameManager.IsGameOver())
    {
        gameManager.Update();
    }

    std::cout << gameManager.GetScores() << std::endl;
}
#endif