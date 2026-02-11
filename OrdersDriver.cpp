#include <iostream>
#include "Orders.h"

int main() {
    std::cout << "=== Orders Driver Test ===\n\n";

    // 1️⃣ Create OrdersList container
    OrdersList ordersList;

    // 2️⃣ Create orders of every kind
    Order* deployOrder   = new Deploy();
    Order* advanceOrder  = new Advance();
    Order* bombOrder     = new Bomb();
    Order* blockadeOrder = new Blockade();
    Order* airliftOrder  = new Airlift();

    // 3️⃣ Add orders to OrdersList sequentially
    ordersList.addOrder(deployOrder);
    ordersList.addOrder(advanceOrder);
    ordersList.addOrder(bombOrder);
    ordersList.addOrder(blockadeOrder);
    ordersList.addOrder(airliftOrder);

    std::cout << "Initial orders in OrdersList:\n";
    ordersList.printOrders();
    std::cout << std::endl;

    // 4️⃣ Validate and execute first order
    std::cout << "Validating and executing first order:\n";
    if (ordersList.getOrder(0)->validate()) {
        ordersList.getOrder(0)->execute();
    }
    std::cout << ordersList.getOrder(0)->getOrderType() 
              << " | Effect: " << ordersList.getOrder(0)->getEffect() << "\n\n";

    // 5️⃣ Move the last order (index 4) to position 1
    ordersList.move(4, 1);
    std::cout << "After moving last order to position 1:\n";
    ordersList.printOrders();
    std::cout << std::endl;

    // 6️⃣ Remove order at index 2
    ordersList.remove(2);
    std::cout << "After removing order at index 2:\n";
    ordersList.printOrders();
    std::cout << std::endl;

    // 7️⃣ Execute all remaining orders
    std::cout << "Executing all remaining orders:\n";
    for (int i = 0; i < ordersList.size(); ++i) {
        if (ordersList.getOrder(i)->validate()) {
            ordersList.getOrder(i)->execute();
        }
        std::cout << ordersList.getOrder(i)->getOrderType()
                  << " | Effect: " << ordersList.getOrder(i)->getEffect() << "\n";
    }

    std::cout << "\n=== End of Orders Driver Test ===\n";

    // Destructor of OrdersList will clean up all dynamically allocated orders
    return 0;
}
