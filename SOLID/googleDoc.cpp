//bad design

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class DocumentEditor {
private:
    vector<string> documentElements;
    string renderedDocument;

public:
    // Adds text as a plain string
    void addText(string text) {
        documentElements.push_back(text);
    }

    // Adds an image represented by its file path
    void addImage(string imagePath) {
        documentElements.push_back(imagePath);
    }

    // Renders the document by checking the type of each element at runtime
    string renderDocument() {
        if(renderedDocument.empty()) {
            string result;
            for (auto element : documentElements) {
                if (element.size() > 4 && (element.substr(element.size() - 4) == ".jpg" ||
                 element.substr(element.size() - 4) == ".png")) {
                    result += "[Image: " + element + "]" + "\n";
                } else {
                    result += element + "\n";
                }
            }
            renderedDocument = result;
        }
        return renderedDocument;
    }

    void saveToFile() {
        ofstream file("document.txt");
        if (file.is_open()) {
            file << renderDocument();
            file.close();
            cout << "Document saved to document.txt" << endl;
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
};

int main() {
    DocumentEditor editor;
    editor.addText("Hello, world!");
    editor.addImage("picture.jpg");
    editor.addText("This is a document editor.");

    cout << editor.renderDocument() << endl;

    editor.saveToFile();
    
    return 0;
}





//better design

#include <iostream>
#include <vector>
#include <string>
#include<fstream>

using namespace std;

// Abstraction for document elements
class DocumentElement {
public:
    virtual string render() = 0;
};

// Concrete implementation for text elements
class TextElement : public DocumentElement {
private:
    string text;

public:
    TextElement(string text) {
        this->text = text;
    }

    string render() override {
        return text;
    }
};

// Concrete implementation for image elements
class ImageElement : public DocumentElement {
private:
    string imagePath;

public:
    ImageElement(string imagePath) {
        this->imagePath = imagePath;
    }

    string render() override {
        return "[Image: " + imagePath + "]";
    }
};

// NewLineElement represents a line break in the document.
class NewLineElement : public DocumentElement {
public:
    string render() override {
        return "\n";
    }
};

// TabSpaceElement represents a tab space in the document.
class TabSpaceElement : public DocumentElement {
public:
    string render() override {
        return "\t";
    }
};

// Document class responsible for holding a collection of elements
class Document {
private:
    vector<DocumentElement*> documentElements;

public:
    void addElement(DocumentElement* element) {
        documentElements.push_back(element);
    }

    // Renders the document by concatenating the render output of all elements.
    string render() {
        string result;
        for (auto element : documentElements) {
            result += element->render();
        }
        return result;
    }
};

// Persistence abstraction
class Persistence {
public:
    virtual void save(string data) = 0;
};

// FileStorage implementation of Persistence
class FileStorage : public Persistence {
public:
    void save(string data) override {
        ofstream outFile("document.txt");
        if (outFile) {
            outFile << data;
            outFile.close();
            cout << "Document saved to document.txt" << endl;
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
};

// Placeholder DBStorage implementation
class DBStorage : public Persistence {
public:
    void save(string data) override {
        // Save to DB
    }
};

// DocumentEditor class managing client interactions
class DocumentEditor {
private:
    Document* document;
    Persistence* storage;
    string renderedDocument;

public:
    DocumentEditor(Document* document, Persistence* storage) {
        this->document = document;
        this->storage = storage;
    }

    void addText(string text) {
        document->addElement(new TextElement(text));
    }

    void addImage(string imagePath) {
        document->addElement(new ImageElement(imagePath));
    }

    // Adds a new line to the document.
    void addNewLine() {
        document->addElement(new NewLineElement());
    }

    // Adds a tab space to the document.
    void addTabSpace() {
        document->addElement(new TabSpaceElement());
    }

    string renderDocument() {
        if(renderedDocument.empty()) {
            renderedDocument = document->render();
        }
        return renderedDocument;
    }

    void saveDocument() {
        storage->save(renderDocument());
    }
};

// Client usage example
int main() {
    Document* document = new Document();
    Persistence* persistence = new FileStorage();

    DocumentEditor* editor = new DocumentEditor(document, persistence);

    // Simulate a client using the editor with common text formatting features.
    editor->addText("Hello, world!");
    editor->addNewLine();
    editor->addText("This is a real-world document editor example.");
    editor->addNewLine();
    editor->addTabSpace();
    editor->addText("Indented text after a tab space.");
    editor->addNewLine();
    editor->addImage("picture.jpg");

    // Render and display the final document.
    cout << editor->renderDocument() << endl;

    editor->saveDocument();

    return 0;
}