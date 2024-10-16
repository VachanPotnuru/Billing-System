#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTS 100
#define DISCOUNT_RATE 0.05
#define DISCOUNT_THRESHOLD 1000.0

struct Product {
    int id;
    char name[50];
    float price;
    int stock;
};

struct Product products[MAX_PRODUCTS];
int productCount = 0;

int isProductIDUnique(int id) {
    int i;
    for (i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            return 0;
        }
    }
    return 1;
}

int getValidInt() {
    int value;
    while (1) {
        if (scanf("%d", &value) == 1) {
            while (getchar() != '\n');
            return value;
        } else {
            printf("Invalid input! Please enter a valid integer: ");
            while (getchar() != '\n');
        }
    }
}

float getValidFloat() {
    float value;
    while (1) {
        if (scanf("%f", &value) == 1) {
            while (getchar() != '\n');
            return value;
        } else {
            printf("Invalid input! Please enter a valid numeric price: ");
            while (getchar() != '\n');
        }
    }
}

void addProduct() {
    if (productCount >= MAX_PRODUCTS) {
        printf("Product list is full!\n");
        return;
    }

    int productId;
    printf("Enter product ID: ");
    productId = getValidInt();

    while (!isProductIDUnique(productId)) {
        printf("Product ID already exists. Please enter a unique ID: ");
        productId = getValidInt();
    }

    products[productCount].id = productId;

    printf("Enter product name: ");
    fgets(products[productCount].name, sizeof(products[productCount].name), stdin);
    products[productCount].name[strcspn(products[productCount].name, "\n")] = '\0';

    printf("Enter product price (in INR): ");
    products[productCount].price = getValidFloat();

    printf("Enter product stock quantity: ");
    products[productCount].stock = getValidInt();

    productCount++;
    printf("Product added successfully!\n");
}

void viewProducts() {
    if (productCount == 0) {
        printf("No products available.\n");
        return;
    }

    printf("\nAvailable Products:\n");
    int i;
    for (i = 0; i < productCount; i++) {
        printf("ID: %d, Name: %s, Price: INR %.2f, Stock: %d\n",
               products[i].id, products[i].name, products[i].price, products[i].stock);
    }
}

void updateStock() {
    int productId, quantity;
    printf("Enter Product ID to update stock: ");
    productId = getValidInt();

    int i;
    for (i = 0; i < productCount; i++) {
        if (products[i].id == productId) {
            printf("Current stock for %s: %d\n", products[i].name, products[i].stock);
            printf("Enter quantity to add: ");
            quantity = getValidInt();

            if (quantity > 0) {
                products[i].stock += quantity;
                printf("Stock updated successfully. New stock for %s: %d\n", products[i].name, products[i].stock);
            } else {
                printf("Invalid quantity! Must be greater than zero.\n");
            }
            return;
        }
    }

    printf("Product with ID %d not found!\n", productId);
}

void createBill() {
    int productId, quantity;
    float total = 0, cashGiven, change;

    printf("\nEnter Product ID to purchase (enter -1 to finish): \n");
    
    struct Product purchasedItems[MAX_PRODUCTS];
    int purchasedCount = 0;

    while (1) {
        printf("Enter Product ID: ");
        productId = getValidInt();
        if (productId == -1) {
            break;
        }

        int found = 0;
        int i;
        for (i = 0; i < productCount; i++) {
            if (products[i].id == productId) {
                found = 1;
                printf("Enter quantity for %s: ", products[i].name);
                quantity = getValidInt();

                if (quantity <= 0) {
                    printf("Invalid quantity! Must be greater than zero.\n");
                } else if (quantity > products[i].stock) {
                    printf("Insufficient stock for product %s!\n", products[i].name);
                } else {
                    products[i].stock -= quantity;

                    int itemIndex = -1;
                    int j;
                    for (j = 0; j < purchasedCount; j++) {
                        if (purchasedItems[j].id == productId) {
                            itemIndex = j;
                            break;
                        }
                    }

                    if (itemIndex != -1) {
                        purchasedItems[itemIndex].stock += quantity;
                    } else {
                        purchasedItems[purchasedCount] = products[i];
                        purchasedItems[purchasedCount].stock = quantity;
                        purchasedCount++;
                    }

                    float cost = products[i].price * quantity;
                    total += cost;
                    printf("Added %d units of %s to the bill. Subtotal: INR %.2f\n", quantity, products[i].name, total);
                }
                break;
            }
        }
        if (!found) {
            printf("Product with ID %d not found!\n", productId);
        }
    }

    if (total > 0) {
        float discount = 0;
        int i;
        if (total > DISCOUNT_THRESHOLD) {
            discount = total * DISCOUNT_RATE;
            total -= discount;
            printf("Discount applied: INR %.2f (5%% off)\n", discount);
        }

        printf("\n=== Bill Summary ===\n");
        printf("ID\tName\t\tQuantity\tCost\n");
        for (i = 0; i < purchasedCount; i++) {
            float cost = purchasedItems[i].price * purchasedItems[i].stock;
            printf("%d\t%s\t\t%d\t\tINR %.2f\n", purchasedItems[i].id, purchasedItems[i].name, purchasedItems[i].stock, cost);
        }
        
        printf("-------------------------------\n");
        printf("Total Bill Amount before discount: INR %.2f\n", total + discount);
        printf("Total Bill Amount after discount: INR %.2f\n", total);

        printf("Enter cash given by customer (in INR): ");
        cashGiven = getValidFloat();

        if (cashGiven >= total) {
            change = cashGiven - total;
            printf("Payment successful!\n");
            printf("Change to return: INR %.2f\n", change);
        } else {
            printf("Insufficient cash provided. Transaction failed.\n");
        }
    } else {
        printf("No items purchased.\n");
    }
}

int main() {
    int choice;

    do {
        printf("\n=== Billing System ===\n");
        printf("1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Update Stock\n");
        printf("4. Create Bill\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        choice = getValidInt();

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                viewProducts();
                break;
            case 3:
                updateStock();
                break;
            case 4:
                createBill();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
