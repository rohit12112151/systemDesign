//SRP followed in this example. Each class has a single responsibility.




#include <iostream>
#include <vector>

using namespace std;

// Product class representing any item in eCommerce.
class Product {
public:
    string name;
    double price;

    Product(string name, double price) {
        this->name = name;
        this->price = price;
    }
};

//1. ShoppingCart: Only responsible for Cart related business logic.
class ShoppingCart {
private:
    vector<Product*> products; // Store heap-allocated products

public:
    void addProduct(Product* p) { 
        products.push_back(p);
    }

    const vector<Product*>& getProducts() { 
        return products;
    } 

    //Calculates total price in cart.
    double calculateTotal() {
        double total = 0;
        for (auto p : products) {
            total += p->price;
        }
        return total;
    }
};

// 2. ShoppingCartPrinter: Only responsible for printing invoices
class ShoppingCartPrinter {
private:
    ShoppingCart* cart; 

public:
    ShoppingCartPrinter(ShoppingCart* cart) { 
        this->cart = cart; 
    }

    void printInvoice() {
        cout << "Shopping Cart Invoice:\n";
        for (auto p : cart->getProducts()) {
            cout << p->name << " - Rs " << p->price << endl;
        }
        cout << "Total: Rs " << cart->calculateTotal() << endl;
    }
};

// 3. ShoppingCartStorage: Only responsible for saving cart to DB
class DBpersistence{
    private:
        ShoppingCart* cart;
    public:
        virtual void saveToDatabase(ShoppingCart* cart) = 0;
};

class saveToMongo: public DBpersistence{
    public:
    void saveToDatabase(ShoppingCart* cart) override{
        cout << "Saving shopping cart to MongoDB..." << endl;
    }
};
class saveToSQL: public DBpersistence{
    public:
    void saveToDatabase(ShoppingCart* cart) override{
        cout << "Saving shopping cart to SQL database..." << endl;
    }
};
class saveToFile: public DBpersistence{
    public:
    void saveToDatabase(ShoppingCart* cart) override{
        cout << "Saving shopping cart to file ..." << endl;
    }
};


int main() {
    ShoppingCart* cart = new ShoppingCart();

    cart->addProduct(new Product("Laptop", 50000));
    cart->addProduct(new Product("Mouse", 2000));

    ShoppingCartPrinter* printer = new ShoppingCartPrinter(cart);
    printer->printInvoice();

    DBpersistence* db = new saveToFile();
    DBpersistence* db1 = new saveToSQL();
    DBpersistence* db2 = new saveToMongo();
    db->saveToDatabase(cart);
    db1->saveToDatabase(cart);
    db2->saveToDatabase(cart);

    return 0;
}