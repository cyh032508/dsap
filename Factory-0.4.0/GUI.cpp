#define USE_GUI
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "PDOGS.cpp"

#include "GameRenderer.hpp"

using namespace Feis;

struct GameRendererConfig
{
    static constexpr int kFPS = 9000;
    static constexpr int kCellSize = 20;
    static constexpr int kBoardLeft = 20;
    static constexpr int kBoardTop = 60;
    static constexpr int kBorderSize = 1;
};

CellPosition GetMouseCellPosition(const sf::RenderWindow &window)
{
    const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    const sf::Vector2i relatedMousePosition =
        mousePosition - sf::Vector2i(GameRendererConfig::kBoardLeft, GameRendererConfig::kBoardTop);
    return {relatedMousePosition.y / GameRendererConfig::kCellSize,
            relatedMousePosition.x / GameRendererConfig::kCellSize};
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

            Feis::CellPosition rightTwoPosition = {pos_y, pos_x + 2};
            Feis::CellPosition rightTwoTopPosition = {pos_y - 1, pos_x + 2};
            Feis::CellPosition rightTwoBottomPosition = {pos_y + 1, pos_x + 2};
            Feis::CellPosition leftTwoPosition = {pos_y, pos_x - 2};
            Feis::CellPosition leftTwoTopPosition = {pos_y - 1, pos_x - 2};
            Feis::CellPosition leftTwoBottomPosition = {pos_y + 1, pos_x - 2};

            Feis::CellPosition topTwoPosition = {pos_y - 2, pos_x};
            Feis::CellPosition bottomTwoPosition = {pos_y + 2, pos_x};

            // 先檢查邊界條件
            bool isTopValid = pos_y > 0;
            bool isBottomValid = pos_y < Feis::GameManagerConfig::kBoardHeight - 1;
            bool isLeftValid = pos_x > 0;
            bool isRightValid = pos_x < Feis::GameManagerConfig::kBoardWidth - 1;

            bool isLeftTwoValid = pos_x > 1;
            bool isLeftTwoTopValid = pos_x > 1 && pos_y > 0;
            bool isLeftTwoBottomValid = pos_x > 1 && pos_y < Feis::GameManagerConfig::kBoardHeight - 1;
            bool isRightTwoValid = pos_x < Feis::GameManagerConfig::kBoardWidth - 2;
            bool isRightTwoTopValid = pos_x < Feis::GameManagerConfig::kBoardWidth - 2 && pos_y > 0;
            bool isRightTwoBottomValid = pos_x < Feis::GameManagerConfig::kBoardWidth - 2 && pos_y < Feis::GameManagerConfig::kBoardHeight - 1;

            bool isTopTwoValid = pos_y > 1;
            bool isBottomTwoValid = pos_y < Feis::GameManagerConfig::kBoardHeight - 2;

            // 初始化指向相邻单元格的指针
            const Feis::NumberCell *numberCell_right = nullptr;
            const Feis::NumberCell *numberCell_left = nullptr;
            const Feis::NumberCell *numberCell_top = nullptr;
            const Feis::NumberCell *numberCell_bottom = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_right = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_left = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_top = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_bottom = nullptr;

            const Feis::NumberCell *numberCell_rightTwo = nullptr;
            const Feis::NumberCell *numberCell_rightTwoTop = nullptr;
            const Feis::NumberCell *numberCell_rightTwoBottom = nullptr;
            const Feis::NumberCell *numberCell_leftTwo = nullptr;
            const Feis::NumberCell *numberCell_leftTwoTop = nullptr;
            const Feis::NumberCell *numberCell_leftTwoBottom = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_rightTwo = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_rightTwoTop = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_rightTwoBottom = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_leftTwo = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_leftTwoTop = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_leftTwoBottom = nullptr;

            const Feis::NumberCell *numberCell_topTwo = nullptr;
            const Feis::NumberCell *numberCell_bottomTwo = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_topTwo = nullptr;
            std::shared_ptr<Feis::ForegroundCell> foregroundCell_bottomTwo = nullptr;

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

            if (isLeftTwoValid)
            {
                const Feis::LayeredCell &layeredCell_LeftTwo = info.GetLayeredCell(leftTwoPosition);
                auto backgroundCell_leftTwo = layeredCell_LeftTwo.GetBackground();
                numberCell_leftTwo = dynamic_cast<const Feis::NumberCell *>(backgroundCell_leftTwo.get());
                foregroundCell_leftTwo = layeredCell_LeftTwo.GetForeground();
            }

            if (isRightTwoValid)
            {
                const Feis::LayeredCell &layeredCell_RightTwo = info.GetLayeredCell(rightTwoPosition);
                auto backgroundCell_rightTwo = layeredCell_RightTwo.GetBackground();
                numberCell_rightTwo = dynamic_cast<const Feis::NumberCell *>(backgroundCell_rightTwo.get());
                foregroundCell_rightTwo = layeredCell_RightTwo.GetForeground();
            }

            if (isLeftTwoTopValid)
            {
                const Feis::LayeredCell &layeredCell_LeftTwoTop = info.GetLayeredCell(leftTwoTopPosition);
                auto backgroundCell_leftTwoTop = layeredCell_LeftTwoTop.GetBackground();
                numberCell_leftTwoTop = dynamic_cast<const Feis::NumberCell *>(backgroundCell_leftTwoTop.get());
                foregroundCell_leftTwoTop = layeredCell_LeftTwoTop.GetForeground();
            }

            if (isLeftTwoBottomValid)
            {
                const Feis::LayeredCell &layeredCell_LeftTwoBottom = info.GetLayeredCell(leftTwoBottomPosition);
                auto backgroundCell_leftTwoBottom = layeredCell_LeftTwoBottom.GetBackground();
                numberCell_leftTwoBottom = dynamic_cast<const Feis::NumberCell *>(backgroundCell_leftTwoBottom.get());
                foregroundCell_leftTwoBottom = layeredCell_LeftTwoBottom.GetForeground();
            }

            if (isRightTwoTopValid)
            {
                const Feis::LayeredCell &layeredCell_RightTwoTop = info.GetLayeredCell(rightTwoTopPosition);
                auto backgroundCell_rightTwoTop = layeredCell_RightTwoTop.GetBackground();
                numberCell_rightTwoTop = dynamic_cast<const Feis::NumberCell *>(backgroundCell_rightTwoTop.get());
                foregroundCell_rightTwoTop = layeredCell_RightTwoTop.GetForeground();
            }

            if (isRightTwoBottomValid)
            {
                const Feis::LayeredCell &layeredCell_RightTwoBottom = info.GetLayeredCell(rightTwoBottomPosition);
                auto backgroundCell_rightTwoBottom = layeredCell_RightTwoBottom.GetBackground();
                numberCell_rightTwoBottom = dynamic_cast<const Feis::NumberCell *>(backgroundCell_rightTwoBottom.get());
                foregroundCell_rightTwoBottom = layeredCell_RightTwoBottom.GetForeground();
            }

            if (isTopTwoValid)
            {
                const Feis::LayeredCell &layeredCell_TopTwo = info.GetLayeredCell(topTwoPosition);
                auto backgroundCell_topTwo = layeredCell_TopTwo.GetBackground();
                numberCell_topTwo = dynamic_cast<const Feis::NumberCell *>(backgroundCell_topTwo.get());
                foregroundCell_topTwo = layeredCell_TopTwo.GetForeground();
            }

            if (isBottomTwoValid)
            {
                const Feis::LayeredCell &layeredCell_BottomTwo = info.GetLayeredCell(bottomTwoPosition);
                auto backgroundCell_bottomTwo = layeredCell_BottomTwo.GetBackground();
                numberCell_bottomTwo = dynamic_cast<const Feis::NumberCell *>(backgroundCell_bottomTwo.get());
                foregroundCell_bottomTwo = layeredCell_BottomTwo.GetForeground();
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

                    int number_rightTwo = 0;
                    if (numberCell_rightTwo != nullptr)
                        number_rightTwo = numberCell_rightTwo->GetNumber();

                    int number_rightTwoTop = 0;
                    if (numberCell_rightTwoTop != nullptr)
                        number_rightTwoTop = numberCell_rightTwoTop->GetNumber();

                    int number_rightTwoBottom = 0;
                    if (numberCell_rightTwoBottom != nullptr)
                        number_rightTwoBottom = numberCell_rightTwoBottom->GetNumber();

                    int number_leftTwo = 0;
                    if (numberCell_leftTwo != nullptr)
                        number_leftTwo = numberCell_leftTwo->GetNumber();

                    int number_leftTwoTop = 0;
                    if (numberCell_leftTwoTop != nullptr)
                        number_leftTwoTop = numberCell_leftTwoTop->GetNumber();

                    int number_leftTwoBottom = 0;
                    if (numberCell_leftTwoBottom != nullptr)
                        number_leftTwoBottom = numberCell_leftTwoBottom->GetNumber();

                    int number_bottomTwo = 0;
                    if (numberCell_bottomTwo != nullptr)
                        number_bottomTwo = numberCell_bottomTwo->GetNumber();
                    
                    int number_topTwo = 0;
                    if (numberCell_topTwo != nullptr)
                        number_topTwo = numberCell_topTwo->GetNumber();

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
                        else if (foregroundCell_right != nullptr and foregroundCell_right->CanRemove() == false)
                            ActivateConveyorFromTopToBottom(action, targetPosition);

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
                            if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false)
                                ActivateConveyorFromBottomToTop(action, targetPosition);
                            else
                                ActivateConveyorFromTopToBottom(action, targetPosition);

                        else if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false and (pos_y == 18 or pos_y == 19))
                            if (foregroundCell_top != nullptr and foregroundCell_top->CanRemove() == false)
                                ActivateConveyorFromTopToBottom(action, targetPosition);
                            else
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
                                    if ((numberCell_leftTwo != nullptr && number_leftTwo == level) || (foregroundCell_leftTwo != nullptr && !foregroundCell_leftTwo->CanRemove()))
                                    {
                                        bool bottomblock = (numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove());
                                        bool topblock = (numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove());
                                        bool leftTwobottomblock = (numberCell_leftTwoBottom != nullptr && number_leftTwoBottom == level) || (foregroundCell_leftTwoBottom != nullptr && !foregroundCell_leftTwoBottom->CanRemove());
                                        bool leftTwotopblock = (numberCell_leftTwoTop != nullptr && number_leftTwoTop == level) || (foregroundCell_leftTwoTop != nullptr && !foregroundCell_leftTwoTop->CanRemove());
                                        if (pos_y < 16)
                                        {
                                            if (!leftTwobottomblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else if (bottomblock && !topblock)
                                                ActivateConveyorFromBottomToTop(action, targetPosition);
                                            else if (bottomblock && !leftTwobottomblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else
                                                ActivateConveyorFromTopToBottom(action, targetPosition);
                                        }
                                        else if (pos_y > 19)
                                        {
                                            if (!leftTwotopblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else if (topblock && !bottomblock)
                                                ActivateConveyorFromTopToBottom(action, targetPosition);
                                            else if (topblock && !leftTwotopblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else
                                                ActivateConveyorFromBottomToTop(action, targetPosition);
                                        }
                                    }
                                    else
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                }
                            }

                            // 左半部
                            else if (pos_x < 29)
                            {
                                if ((numberCell_right != nullptr && number_right == level) || (foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()))
                        {
                            if (foregroundCell_top != nullptr && ((numberCell_top != nullptr && number_top == level) || (!foregroundCell_top->CanRemove())))
                            {
                                if (pos_y >= 19)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                                else if (pos_y <= 18)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }
                            else if (foregroundCell_bottom != nullptr && ((numberCell_bottom != nullptr && number_bottom == level) || (!foregroundCell_bottom->CanRemove())))
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
                            bool righttwotopblock = (numberCell_rightTwoTop != nullptr && number_rightTwoTop == level) || (foregroundCell_rightTwoTop != nullptr && !foregroundCell_rightTwoTop->CanRemove());
                            bool righttwobottomblock = (numberCell_rightTwoBottom != nullptr && number_rightTwoBottom == level) || (foregroundCell_rightTwoBottom != nullptr && !foregroundCell_rightTwoBottom->CanRemove());
                            bool bottomblock = (numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove());
                            bool topblock = (numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove());
                            bool righttwoblock = (numberCell_rightTwo != nullptr && number_rightTwo == level) || (foregroundCell_rightTwo != nullptr && !foregroundCell_rightTwo->CanRemove());
                            bool bottomtwoblock = (numberCell_bottomTwo != nullptr && number_bottomTwo == level) || (foregroundCell_bottomTwo != nullptr && !foregroundCell_bottomTwo->CanRemove());
                            bool toptwoblock = (numberCell_topTwo != nullptr && number_topTwo == level) || (foregroundCell_topTwo != nullptr && !foregroundCell_topTwo->CanRemove());
                            if (righttwoblock)
                            {
                                if (pos_y < 16)
                                {
                                    if (bottomblock && !topblock)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                    else if (bottomblock && !righttwobottomblock)
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                    else if (bottomtwoblock)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                    else
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                }
                                else if (pos_y > 19)
                                {
                                    if (topblock && !bottomblock)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else if (topblock && !righttwotopblock)
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                    else if (toptwoblock)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }
                            }
                            else
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
                                    if ((numberCell_leftTwo != nullptr && number_leftTwo == level) || (foregroundCell_leftTwo != nullptr && !foregroundCell_leftTwo->CanRemove()))
                                    {
                                        bool bottomblock = (numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove());
                                        bool topblock = (numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove());
                                        bool leftTwobottomblock = (numberCell_leftTwoBottom != nullptr && number_leftTwoBottom == level) || (foregroundCell_leftTwoBottom != nullptr && !foregroundCell_leftTwoBottom->CanRemove());
                                        bool leftTwotopblock = (numberCell_leftTwoTop != nullptr && number_leftTwoTop == level) || (foregroundCell_leftTwoTop != nullptr && !foregroundCell_leftTwoTop->CanRemove());
                                        if (pos_y < 16)
                                        {
                                            if (!leftTwobottomblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else if (bottomblock && !topblock)
                                                ActivateConveyorFromBottomToTop(action, targetPosition);
                                            else if (bottomblock && !leftTwobottomblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else
                                                ActivateConveyorFromTopToBottom(action, targetPosition);
                                        }
                                        else if (pos_y > 19)
                                        {
                                            if (!leftTwotopblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else if (topblock && !bottomblock)
                                                ActivateConveyorFromTopToBottom(action, targetPosition);
                                            else if (topblock && !leftTwotopblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else
                                                ActivateConveyorFromBottomToTop(action, targetPosition);
                                        }
                                    }
                                    else
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                }
                            }

                            // 左半部
                            else if (pos_x < 29)
                            {

                                if ((numberCell_right != nullptr && number_right == level) || (foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()))
                        {
                            if (foregroundCell_top != nullptr && ((numberCell_top != nullptr && number_top == level) || (!foregroundCell_top->CanRemove())))
                            {
                                if (pos_y >= 19)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                                else if (pos_y <= 18)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }
                            else if (foregroundCell_bottom != nullptr && ((numberCell_bottom != nullptr && number_bottom == level) || (!foregroundCell_bottom->CanRemove())))
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
                            bool righttwotopblock = (numberCell_rightTwoTop != nullptr && number_rightTwoTop == level) || (foregroundCell_rightTwoTop != nullptr && !foregroundCell_rightTwoTop->CanRemove());
                            bool righttwobottomblock = (numberCell_rightTwoBottom != nullptr && number_rightTwoBottom == level) || (foregroundCell_rightTwoBottom != nullptr && !foregroundCell_rightTwoBottom->CanRemove());
                            bool bottomblock = (numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove());
                            bool topblock = (numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove());
                            bool righttwoblock = (numberCell_rightTwo != nullptr && number_rightTwo == level) || (foregroundCell_rightTwo != nullptr && !foregroundCell_rightTwo->CanRemove());
                            bool bottomtwoblock = (numberCell_bottomTwo != nullptr && number_bottomTwo == level) || (foregroundCell_bottomTwo != nullptr && !foregroundCell_bottomTwo->CanRemove());
                            bool toptwoblock = (numberCell_topTwo != nullptr && number_topTwo == level) || (foregroundCell_topTwo != nullptr && !foregroundCell_topTwo->CanRemove());
                            if (righttwoblock)
                            {
                                if (pos_y < 16)
                                {
                                    if (bottomblock && !topblock)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                    else if (bottomblock && !righttwobottomblock)
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                    else if (bottomtwoblock)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                    else
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                }
                                else if (pos_y > 19)
                                {
                                    if (topblock && !bottomblock)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else if (topblock && !righttwotopblock)
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                    else if (toptwoblock)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }
                            }
                            else
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
                                    if ((numberCell_leftTwo != nullptr && number_leftTwo == level) || (foregroundCell_leftTwo != nullptr && !foregroundCell_leftTwo->CanRemove()))
                                    {
                                        bool bottomblock = (numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove());
                                        bool topblock = (numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove());
                                        bool leftTwobottomblock = (numberCell_leftTwoBottom != nullptr && number_leftTwoBottom == level) || (foregroundCell_leftTwoBottom != nullptr && !foregroundCell_leftTwoBottom->CanRemove());
                                        bool leftTwotopblock = (numberCell_leftTwoTop != nullptr && number_leftTwoTop == level) || (foregroundCell_leftTwoTop != nullptr && !foregroundCell_leftTwoTop->CanRemove());
                                        if (pos_y < 16)
                                        {
                                            if (!leftTwobottomblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else if (bottomblock && !topblock)
                                                ActivateConveyorFromBottomToTop(action, targetPosition);
                                            else if (bottomblock && !leftTwobottomblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else
                                                ActivateConveyorFromTopToBottom(action, targetPosition);
                                        }
                                        else if (pos_y > 19)
                                        {
                                            if (!leftTwotopblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else if (topblock && !bottomblock)
                                                ActivateConveyorFromTopToBottom(action, targetPosition);
                                            else if (topblock && !leftTwotopblock)
                                                ActivateConveyorFromRightToLeft(action, targetPosition);
                                            else
                                                ActivateConveyorFromBottomToTop(action, targetPosition);
                                        }
                                    }
                                    else
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                }
                            }

                            // 左半部
                            else if (pos_x < 29)
                            {
                                if ((numberCell_right != nullptr && number_right == level) || (foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()))
                        {
                            if (foregroundCell_top != nullptr && ((numberCell_top != nullptr && number_top == level) || (!foregroundCell_top->CanRemove())))
                            {
                                if (pos_y >= 19)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                                else if (pos_y <= 18)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }
                            else if (foregroundCell_bottom != nullptr && ((numberCell_bottom != nullptr && number_bottom == level) || (!foregroundCell_bottom->CanRemove())))
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
                            bool righttwotopblock = (numberCell_rightTwoTop != nullptr && number_rightTwoTop == level) || (foregroundCell_rightTwoTop != nullptr && !foregroundCell_rightTwoTop->CanRemove());
                            bool righttwobottomblock = (numberCell_rightTwoBottom != nullptr && number_rightTwoBottom == level) || (foregroundCell_rightTwoBottom != nullptr && !foregroundCell_rightTwoBottom->CanRemove());
                            bool bottomblock = (numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove());
                            bool topblock = (numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove());
                            bool righttwoblock = (numberCell_rightTwo != nullptr && number_rightTwo == level) || (foregroundCell_rightTwo != nullptr && !foregroundCell_rightTwo->CanRemove());
                            bool bottomtwoblock = (numberCell_bottomTwo != nullptr && number_bottomTwo == level) || (foregroundCell_bottomTwo != nullptr && !foregroundCell_bottomTwo->CanRemove());
                            bool toptwoblock = (numberCell_topTwo != nullptr && number_topTwo == level) || (foregroundCell_topTwo != nullptr && !foregroundCell_topTwo->CanRemove());
                            if (righttwoblock)
                            {
                                if (pos_y < 16)
                                {
                                    if (bottomblock && !topblock)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                    else if (bottomblock && !righttwobottomblock)
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                    else if (bottomtwoblock)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                    else
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                }
                                else if (pos_y > 19)
                                {
                                    if (topblock && !bottomblock)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else if (topblock && !righttwotopblock)
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                    else if (toptwoblock)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }
                            }
                            else
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
                        if (foregroundCell_bottom != nullptr and foregroundCell_bottom->CanRemove() == false)
                            ActivateConveyorFromBottomToTop(action, targetPosition);
                        else
                            ActivateConveyorFromTopToBottom(action, targetPosition);

                    else if (foregroundCell_left != nullptr and foregroundCell_left->CanRemove() == false and (pos_y == 18 or pos_y == 19))
                        if (foregroundCell_top != nullptr and foregroundCell_top->CanRemove() == false)
                            ActivateConveyorFromTopToBottom(action, targetPosition);
                        else
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

                    int number_leftTwo = 0;
                    if (numberCell_leftTwo != nullptr)
                        number_leftTwo = numberCell_leftTwo->GetNumber();

                    int number_leftTwoTop = 0;
                    if (numberCell_leftTwoTop != nullptr)
                        number_leftTwoTop = numberCell_leftTwoTop->GetNumber();

                    int number_leftTwoBottom = 0;
                    if (numberCell_leftTwoBottom != nullptr)
                        number_leftTwoBottom = numberCell_leftTwoBottom->GetNumber();

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
                                if ((numberCell_leftTwo != nullptr && number_leftTwo == level) || (foregroundCell_leftTwo != nullptr && !foregroundCell_leftTwo->CanRemove()))
                                {
                                    if (pos_y < 16)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else if (pos_y > 19)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }
                                else
                                    ActivateConveyorFromRightToLeft(action, targetPosition);
                            }
                        }
                    }

                    else
                    {
                        // commondividor == 2 || 3 || 5
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
                            if ((numberCell_leftTwo != nullptr && number_leftTwo == level) || (foregroundCell_leftTwo != nullptr && !foregroundCell_leftTwo->CanRemove()))
                            {
                                bool bottomblock = (numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove());
                                bool topblock = (numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove());
                                bool leftTwobottomblock = (numberCell_leftTwoBottom != nullptr && number_leftTwoBottom == level) || (foregroundCell_leftTwoBottom != nullptr && !foregroundCell_leftTwoBottom->CanRemove());
                                bool leftTwotopblock = (numberCell_leftTwoTop != nullptr && number_leftTwoTop == level) || (foregroundCell_leftTwoTop != nullptr && !foregroundCell_leftTwoTop->CanRemove());
                                if (pos_y < 16)
                                {
                                    if (!leftTwobottomblock)
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                    else if (bottomblock && !topblock)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                    else if (bottomblock && !leftTwobottomblock)
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                    else
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                }
                                else if (pos_y > 19)
                                {
                                    if (!leftTwotopblock)
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                    else if (topblock && !bottomblock)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else if (topblock && !leftTwotopblock)
                                        ActivateConveyorFromRightToLeft(action, targetPosition);
                                    else
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }
                            }
                            else
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

                    int number_rightTwo = 0;
                    if (numberCell_rightTwo != nullptr)
                        number_rightTwo = numberCell_rightTwo->GetNumber();

                    int number_rightTwoTop = 0;
                    if (numberCell_rightTwoTop != nullptr)
                        number_rightTwoTop = numberCell_rightTwoTop->GetNumber();

                    int number_rightTwoBottom = 0;
                    if (numberCell_rightTwoBottom != nullptr)
                        number_rightTwoBottom = numberCell_rightTwoBottom->GetNumber();

                    int number_bottomTwo = 0;
                    if (numberCell_bottomTwo != nullptr)
                        number_bottomTwo = numberCell_bottomTwo->GetNumber();
                    
                    int number_topTwo = 0;
                    if (numberCell_topTwo != nullptr)
                        number_topTwo = numberCell_topTwo->GetNumber();

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
                        if ((numberCell_right != nullptr && number_right == level) || (foregroundCell_right != nullptr && !foregroundCell_right->CanRemove()))
                        {
                            if (foregroundCell_top != nullptr && ((numberCell_top != nullptr && number_top == level) || (!foregroundCell_top->CanRemove())))
                            {
                                if (pos_y >= 19)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                                else if (pos_y <= 18)
                                    ActivateConveyorFromTopToBottom(action, targetPosition);
                            }
                            else if (foregroundCell_bottom != nullptr && ((numberCell_bottom != nullptr && number_bottom == level) || (!foregroundCell_bottom->CanRemove())))
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
                            bool righttwotopblock = (numberCell_rightTwoTop != nullptr && number_rightTwoTop == level) || (foregroundCell_rightTwoTop != nullptr && !foregroundCell_rightTwoTop->CanRemove());
                            bool righttwobottomblock = (numberCell_rightTwoBottom != nullptr && number_rightTwoBottom == level) || (foregroundCell_rightTwoBottom != nullptr && !foregroundCell_rightTwoBottom->CanRemove());
                            bool bottomblock = (numberCell_bottom != nullptr && number_bottom == level) || (foregroundCell_bottom != nullptr && !foregroundCell_bottom->CanRemove());
                            bool topblock = (numberCell_top != nullptr && number_top == level) || (foregroundCell_top != nullptr && !foregroundCell_top->CanRemove());
                            bool righttwoblock = (numberCell_rightTwo != nullptr && number_rightTwo == level) || (foregroundCell_rightTwo != nullptr && !foregroundCell_rightTwo->CanRemove());
                            bool bottomtwoblock = (numberCell_bottomTwo != nullptr && number_bottomTwo == level) || (foregroundCell_bottomTwo != nullptr && !foregroundCell_bottomTwo->CanRemove());
                            bool toptwoblock = (numberCell_topTwo != nullptr && number_topTwo == level) || (foregroundCell_topTwo != nullptr && !foregroundCell_topTwo->CanRemove());
                            if (righttwoblock)
                            {
                                if (pos_y < 16)
                                {
                                    if (bottomblock && !topblock)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                    else if (bottomblock && !righttwobottomblock)
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                    else if (bottomtwoblock)
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                    else
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                }
                                else if (pos_y > 19)
                                {
                                    if (topblock && !bottomblock)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else if (topblock && !righttwotopblock)
                                        ActivateConveyorFromLeftToRight(action, targetPosition);
                                    else if (toptwoblock)
                                        ActivateConveyorFromTopToBottom(action, targetPosition);
                                    else
                                        ActivateConveyorFromBottomToTop(action, targetPosition);
                                }
                            }
                            else
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

void Save(std::queue<PlayerAction> playerActionHistory, const std::string &filename)
{
    std::ofstream outFile(filename);
    while (!playerActionHistory.empty())
    {
        const PlayerAction &playerAction = playerActionHistory.front();
        outFile << playerAction.cellPosition.row << " " << playerAction.cellPosition.col << " " << static_cast<int>(playerAction.type) << std::endl;
        playerActionHistory.pop();
    }
}

int main(int, char **)
{
    sf::VideoMode mode = sf::VideoMode(1280, 1024);

    sf::RenderWindow window(mode, "DSAP Final Project", sf::Style::Close);

    window.setFramerateLimit(GameRendererConfig::kFPS);

    GamePlayer player;

    GameManager gameManager(&player, 4, 35);

    const std::map<sf::Keyboard::Key, PlayerActionType> playerActionKeyboardMap = {
        {sf::Keyboard::J, PlayerActionType::BuildLeftOutMiningMachine},
        {sf::Keyboard::I, PlayerActionType::BuildTopOutMiningMachine},
        {sf::Keyboard::L, PlayerActionType::BuildRightOutMiningMachine},
        {sf::Keyboard::K, PlayerActionType::BuildBottomOutMiningMachine},
        {sf::Keyboard::D, PlayerActionType::BuildLeftToRightConveyor},
        {sf::Keyboard::S, PlayerActionType::BuildTopToBottomConveyor},
        {sf::Keyboard::A, PlayerActionType::BuildRightToLeftConveyor},
        {sf::Keyboard::W, PlayerActionType::BuildBottomToTopConveyor},
        {sf::Keyboard::Num1, PlayerActionType::BuildTopOutCombiner},
        {sf::Keyboard::Num2, PlayerActionType::BuildRightOutCombiner},
        {sf::Keyboard::Num3, PlayerActionType::BuildBottomOutCombiner},
        {sf::Keyboard::Num4, PlayerActionType::BuildLeftOutCombiner},
        {sf::Keyboard::Backspace, PlayerActionType::Clear},
    };

    PlayerActionType playerActionType = PlayerActionType::BuildLeftToRightConveyor;

    GameRenderer<GameRendererConfig> gameRenderer(&window);

    std::queue<PlayerAction> playerActionHistory;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            /*if (event.type == sf::Event::MouseButtonReleased)
            {
                CellPosition mouseCellPosition = GetMouseCellPosition(window);

                if (IsWithinBoard(mouseCellPosition))
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        auto playerAction = PlayerAction{playerActionType, mouseCellPosition};
                        player.EnqueueAction(playerAction);
                        playerActionHistory.push(playerAction);
                    }
                }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (playerActionKeyboardMap.count(event.key.code))
                {
                    playerActionType = playerActionKeyboardMap.at(event.key.code);
                }
                else if (event.key.code == sf::Keyboard::F4)
                {
                    Save(playerActionHistory, "gameplay.txt");
                }
            }*/
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        gameManager.Update();

        gameRenderer.Render(gameManager);
    }
}
// void Test1A() { Test(1, 20); }
// void Test1B() { Test(1, 0 /* HIDDEN */); }

// void Test2A() { Test(2, 25); }
// void Test2B() { Test(2, 0 /* HIDDEN */); }

// void Test3A() { Test(3, 30); }
// void Test3B() { Test(3, 0 /* HIDDEN */); }

// void Test4A() { Test(4, 35); }
// void Test4B() { Test(4, 0 /* HIDDEN */); }

// void Test5A() { Test(5, 40); }
// void Test5B() { Test(5, 0 /* HIDDEN */); }