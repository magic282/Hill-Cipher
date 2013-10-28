/* 
 * File:   main.cpp
 * Author: magic282
 *
 * Created on 2013,9,28, 12:28
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
// A third party linear algebra library
#include <Eigen/Dense>

//#define DEBUG
//define for convert between char and int
#define char2int(x) (x - 'A')
#define int2char(x) ((char)'A' + x)

using namespace Eigen;
using namespace std;

/*
 * 
 */

int choice; // 1 for encrypt, 2 for decrypt
string matFileName; // matrix file name
string fileName; // plain or cipher text file name
int mSize; // matrix size
MatrixXi m; // key matrix
VectorXi v; // store text code
string plaintext;
string ciphertext;

// Get the key matrix
// it can read from console or file

void getKeyMat() {
    m.resize(mSize, mSize);

    if (matFileName.length() == 0) {
        for (int i = 0; i < mSize; ++i) {
            for (int j = 0; j < mSize; ++j) {
                cin >> m(i, j);
            }
        }
    } else {
        ifstream fin;
        fin.open(matFileName.c_str());
        if (fin.is_open()) {
            for (int i = 0; i < mSize; ++i) {
                for (int j = 0; j < mSize; ++j) {
                    fin >> m(i, j);
                }
            }
        } else {
            perror("Error: Unable to open the key matrix file\n");
            exit(0);
        }
        fin.close();
    }
    printf("getKeyMat done.\n");
    cout << "Key matrix is " << endl << m << endl;
}

// According to the project description 
// the program should get the plaintext
// or cipher text from file
// Get the plaintext or ciphertext from file

void getText() {
    ifstream in;
    in.open(fileName.c_str());
    if (!in.is_open()) {
        perror("Error: Unable to open the text file\n");
        exit(0);
    }
    in >> plaintext;
    in.close();
    printf("getText done.\n");
}

// Save the encrypted or decrypted text to file
// The file is named using a suffix
// .enciphered
// or
// .deciphered

void saveFile() {
    ofstream out;
    string fstr;
    if (choice == 1) {
        fstr = fileName + ".enciphered";
    } else {
        fstr = fileName + ".deciphered";
    }
    out.open(fstr.c_str());
    out << ciphertext;
    out.close();
    printf("saveFile to %s done.\n", fstr.c_str());
    printf("\n");
    if (choice == 1) {
        printf("The plaintext is:\n%s\n", plaintext.c_str());
        printf("The ciphertext is:\n%s\n", ciphertext.c_str());
    } else {
        printf("The ciphertext is:\n%s\n", plaintext.c_str());
        printf("The plaintext is:\n%s\n", ciphertext.c_str());
    }

}

// encrypt the plaintext with the key
// In fact, because the Hill cipher does 
// the same thing for encryption and decryption
// the only difference is the matrix
// so the function is also used to decryption
// But called by function decipher();

void encipher() {
    getText();
    v.resize(mSize);
    ciphertext = "";
    // Add "X" to the tail of the text if
    // the matrix size cannot divide the length of text
    while (plaintext.length() % mSize != 0) {
        plaintext += "X";
    }
    if (choice == 1) {
        cout << "key matrix is " << endl << m << endl;
    }
    int rounds = plaintext.length() / mSize;
    VectorXi tmp(mSize);
    for (int i = 0; i < rounds; ++i) {
        for (int j = 0; j < mSize; ++j) {
            // Store the text code to vector v
            v(j) = char2int(plaintext[i * mSize + j]);
        }
        // Encrypt
        tmp = m*v;
        // Modulo 26
        for (int j = 0; j < mSize; ++j) {
            while (tmp(j) < 0)
                tmp(j) += 26;
            tmp(j) %= 26;
        }
        for (int j = 0; j < mSize; ++j) {
            // Convert to English char
            ciphertext += int2char(tmp(j));
        }
    }
    if (choice == 1) {
        printf("encipher done!\n");
    } else {
        printf("decipher done!\n");
    }
    saveFile();
}

// Use Extended Euclidean Algorithm to get the x

inline int gcd(int a, int b) {
    bool flag = false;
    if (b > a) {
        flag = true;
        swap(a, b);
    }
    int x = 1;
    int y = 0;
    int g = a;
    int r = 0;
    int s = 1;
    int t = b;
    while (t > 0) {
        int q = g / t;
        int u = x - q*r;
        int v = y - q*s;
        int w = g - q*t;
        x = r;
        y = s;
        g = t;
        r = u;
        s = v;
        t = w;
    }
    if (flag) {
        return y;
    } else {
        return x;
    }
}

// The divide operation with modulo

inline int modDiv(int a, int d) {

    if (a == d) {
        return 1;
    }
    if (d == 1) {
        return a % 26;
    }
    int x = gcd(d, 26);

    x = (x < 0 ? x + 26 : x);
    return ((a * x) % 26);
}

// Inverse the key matrix
// Use Gaussian Elimination algorithm

void inverseKey() {
    // this part is core code and related to a course project.
    // so it should not be public.
    // if interested and NOT for the project, one can email me.
}

// Decrypt function
// Just simply call inverseKey and encipher

void decipher() {
    inverseKey();
    printf("The inversed key matrix is:\n");
    cout << m << endl;
    encipher();

}

int main(int argc, char** argv) {
    // Get parameters from 
    // Can work using terminal arguments 
    // or
    // interact with user in console
    if (argc == 5) {
        mSize = atoi(argv[1]);
        choice = argv[2][0] - '0';
        matFileName = string(argv[3]);
        fileName = string(argv[4]);
        getKeyMat();

    } else {
        printf("You can also use it this way under linux terminal\n");
        printf("./main 4 1 matC plain2\n");
        printf("4 means the matrix size\n");
        printf("1 means encrypt, 2 means decrypt\n");
        printf("matC is the name of your key matrix filename\n");
        printf("plain2 is the name of your plain/cipher text\n");
        printf("---------------------------------------------\n\n");
        matFileName = string("");

        printf("Please input the matrix size:\n");
        cin >> mSize;
        printf("press 1 for encipher, press 2 for decipher\n");
        cin >> choice;

        printf("Please input the key matrix, row by row\n");
        getKeyMat();

        printf("Please input the filename you want to encipher/ decipher\n");
        cin >> fileName;
    }

    if (choice == 1) {
        encipher();
    } else {
        decipher();
    }

    return 0;
}
