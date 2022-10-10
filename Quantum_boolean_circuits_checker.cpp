#include <iostream>
using namespace std;
int g_num,q_num;
int qState_num=1;
char **circuit;
char **origin_qubit;
char **qubit;
void draw_circuit();
void dec_output();
void hex_output();
void oct_output();
void bin_output();
void process_qubit();
void generate_qState();
bool input_circuit();
void discribe_rules();
int main(){
    string represent;
    discribe_rules();
    if(input_circuit()){
        draw_circuit();
        generate_qState();
        process_qubit();
        cout << "\n\nPlease choose the truth table representation\n(bin/oct/hex/dec): ";
        cin >> represent;
        cout << "\n";
        if(!represent.compare("bin"))
            bin_output();
        else if(!represent.compare("oct"))
            oct_output();
        else if(!represent.compare("hex"))
            hex_output();
        else if(!represent.compare("dec"))
            dec_output();
        else 
            cout << "Invalid input of representation\n";
    }
    else
        cout << "Invalid input of circuit\n";
    return 0;
}
void discribe_rules(){
    cout << "The input rules and descriptions of input numbers are listed below:\n\n";
    cout << "Input\tDescription\n";
    cout << "0\tControl bit(low level trigger)\n";
    cout << "1\tControl bit(high level trigger)\n";
    cout << "2\tNOT-gate\n";
    cout << "3\tWire gate\n\n";
    cout << "Input rules: \n";
    cout << "1. Each row represents a gate\n";
    cout << "2. The number of columns represents the number of input Qubits\n";
    cout << "3. It's invalid that a gate only has control bits and wire gate and without NOT-gate\n";
    cout << "4. Please seperate each input with a blank\n";
    cout << "5. After the end of each row, please add a newline\n\n";
}
bool input_circuit(){
    cout << "Input the number of gate: ";
    cin >> g_num;
    cout << "Input the number of Qubit: ";
    cin >> q_num;
    cout << "Input the circuit: \n";
    char c;
    c = getchar();//read \n
    circuit=new char*[g_num];
    bool valid=true;
    int tmp;
    for (int i = 0; i < g_num;i++){//judge if it's valid input
        circuit[i] = new char[q_num];
        tmp = 0;
        for (int j = 0; j < q_num;j++){
            c = getchar();
            if(c==' '||c=='\n'||(c-'0')<0||(c-'0')>3){
                valid = false;
                break;
            }
            else{
                circuit[i][j] = c;
                if(c=='2')
                    tmp++;
            }
            c = getchar();
            if((j==q_num-1&&c=='\n')||(j!=q_num-1&&c==' '))
                valid = true;
            else {
                valid = false;
                break;
            }
        }
        if(!tmp)
            valid = false;
        if(!valid)
            break;
    }
    if(!valid)
        return false;
    else
        return true;
}
void generate_qState(){
    int tmp;
    for (int i = 0; i < q_num;i++)
        qState_num *= 2;
    origin_qubit = new char *[qState_num];    
    qubit = new char *[qState_num];
    for (int i = 0; i < qState_num;i++){
        origin_qubit[i] = new char[q_num];
        qubit[i] = new char[q_num];
        tmp = i;
        for (int j = q_num - 1; j >= 0; j--){
            origin_qubit[i][j] = tmp % 2 + '0';
            qubit[i][j] = origin_qubit[i][j];
            tmp /= 2;
        }
    }
}
void process_qubit(){
    bool chg;
    for (int i = 0; i < qState_num;i++){
        for (int j = 0; j < g_num; j++){
            chg = true;
            for (int k = 0; k < q_num; k++){
                if((circuit[j][k]=='0'&&qubit[i][k]=='1')||(circuit[j][k]=='1'&&qubit[i][k]=='0')){
                    chg = false;
                    break;
                }
            }
            if(chg){
                for (int k = 0; k < q_num; k++){
                    if(circuit[j][k]=='2'){
                        if(qubit[i][k]=='0')
                            qubit[i][k] = '1';
                        else 
                            qubit[i][k] = '0';
                    }
                }
            }
        }
    }
}
void bin_output(){
    cout << "Truth table in binary representation:\n";
    for (int i = 0; i < qState_num;i++){
        for (int j = 0; j < q_num;j++)
            cout << origin_qubit[i][j];
        cout << " -> ";
        for (int j = 0; j < q_num;j++)
            cout << qubit[i][j];
        cout << "\n";
    }
}
void dec_output(){
    cout << "Truth table in decimal representation:\n";
    int tmp,itr;
    for (int i = 0; i < qState_num;i++){
        tmp = 0;
        itr = 1;
        for (int j = q_num - 1; j >=0; j--){
            tmp+=itr*(origin_qubit[i][j]-'0');
            itr *= 2;
        }
        cout << tmp << " -> ";
        tmp = 0;
        itr = 1;
        for (int j = q_num - 1; j >=0; j--){
            tmp+=itr*(qubit[i][j]-'0');
            itr *= 2;
        }
        cout << tmp << "\n";
    }
}
void oct_output(){
    cout << "Truth table in octal representation:\n";
    int rest = q_num % 3, itr, tmp;
    for (int i = 0; i < qState_num; i++){
        itr = 1;
        tmp = 0;
        for (int j = rest-1; j>=0;j--){
            tmp += (origin_qubit[i][j]-'0') * itr;
            itr *= 2;
        }
        if(tmp)
            cout << tmp;
        for (int j = rest; j < q_num; j+=3){
            tmp = 0;
            tmp += (origin_qubit[i][j]-'0') * 4;
            tmp += (origin_qubit[i][j+1]-'0') * 2;
            tmp += (origin_qubit[i][j+2]-'0') * 1;
            cout << tmp;
        }
        cout << " -> ";
        itr = 1;
        tmp = 0;
        for (int j = rest-1; j>=0;j--){
            tmp += (qubit[i][j]-'0') * itr;
            itr *= 2;
        }
        if(tmp)
            cout << tmp;
        for (int j = rest; j < q_num; j+=3){
            tmp = 0;
            tmp += (qubit[i][j]-'0') * 4;
            tmp += (qubit[i][j+1]-'0') * 2;
            tmp += (qubit[i][j+2]-'0') * 1;
            cout << tmp;
        }
        cout << "\n";
    }
}
void hex_output(){
    cout << "Truth table in hexidecimal representation:\n";
    int rest = q_num % 4, itr, tmp;
    for (int i = 0; i < qState_num; i++){
        itr = 1;
        tmp = 0;
        for (int j = rest-1; j>=0;j--){
            tmp += (origin_qubit[i][j]-'0') * itr;
            itr *= 2;
        }
        if(tmp)
            cout << tmp;
        for (int j = rest; j < q_num; j+=4){
            tmp = 0;
            tmp += (origin_qubit[i][j]-'0') * 8;
            tmp += (origin_qubit[i][j+1]-'0') * 4;
            tmp += (origin_qubit[i][j+2]-'0') * 2;
            tmp += (origin_qubit[i][j+3]-'0') * 1;
            if(tmp>=10)
                cout << char(tmp+55);
            else 
                cout << tmp;
        }
        cout << " -> ";
        itr = 1;
        tmp = 0;
        for (int j = rest-1; j>=0;j--){
            tmp += (qubit[i][j]-'0') * itr;
            itr *= 2;
        }
        if(tmp)
            cout << tmp;
        for (int j = rest; j < q_num; j+=4){
            tmp = 0;
            tmp += (qubit[i][j]-'0') * 8;
            tmp += (qubit[i][j+1]-'0') * 4;
            tmp += (qubit[i][j+2]-'0') * 2;
            tmp += (qubit[i][j+3]-'0') * 1;
            if(tmp>=10)
                cout << char(tmp+55);
            else 
                cout << tmp;
        }
        cout << "\n";
    }
}
void draw_circuit(){
    cout << "\nCircuit diagram\n(@: low-level control bit)\n(O: high-level control bit)\n(N: not-gate)\n";
    for (int i = 0; i < q_num;i++){
        cout << "q" << i;
        for (int j = 0; j < g_num;j++){
            switch(circuit[j][i]){
                case '0':
                    cout << "--O-";
                    break;
                case '1':
                    cout << "--@-";
                    break;
                case '2':
                    cout << "--N-";
                    break;
                case '3':
                    cout << "----";
                    break;
            }
            if(j==g_num-1&&i!=q_num-1){
                cout << "\n";
                for (int k = 0; k < g_num;k++){
                    if(k==0)
                        cout << "    | ";
                    else
                        cout << "  | ";
                }
                cout << "\n";
            }
        }
    }
}
