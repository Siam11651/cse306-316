#include <bits/stdc++.h>
#include <vector>
#include<math.h>
#include<string.h>
using namespace std;
#define MAX 300005
#define MOD 1000000007
#define INF 1000000007
#define EPS 0.0000000001
#define CHAINS 18
#define FASTIO ios_base::sync_with_stdio(false);cin.tie(NULL)
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/detail/standard_policies.hpp>
map<string,int> opcodes_mapping;
map<string,int> registers_mapping;
const string Push="push";
const string Pop="pop";
vector<int> out;
map<string,int> labeling_number;
map<int,int> label_position;
queue<pair<int,int> > Q;
char opcode_format[16];
int sp_increment,sp_decrement;
int labels,temp_reg;
int load,store;
char hex_digits[16];
void initialize_registers()
{
    labels=0;
    registers_mapping["zero"]=6;
    registers_mapping["t0"]=0;
    registers_mapping["t1"]=1;
    registers_mapping["t2"]=2;
    registers_mapping["t3"]=3;
    registers_mapping["t4"]=4;
    registers_mapping["t5"]=5;
    temp_reg=registers_mapping["t5"];
    registers_mapping["sp"]=7;
}
void stack_initialize()
{
    int opcode=opcodes_mapping["addi"];
    opcode=opcode<<16;
    int dst=registers_mapping["sp"];
    dst=dst<<8;
    int src=registers_mapping["zero"];
    src=src<<12;
    int con=255;
    int ans=opcode|src|dst|con;
    out.push_back(ans);
}
void initialize_opcodes()
{
    char permutation[17]="GBFOLNAIEDJMKHCP";
    int arr[16];
    for(int i=0; i<16; i++)
    {
        arr[permutation[i]-'A']=i;
    }
    opcode_format[arr[0]]='R';
    opcode_format[arr[1]]='I';
    opcode_format[arr[2]]='R';
    opcode_format[arr[3]]='I';
    opcode_format[arr[4]]='R';
    opcode_format[arr[5]]='I';
    opcode_format[arr[6]]='R';
    opcode_format[arr[7]]='I';
    opcode_format[arr[8]]='r';
    opcode_format[arr[9]]='r';
    opcode_format[arr[10]]='R';
    opcode_format[arr[11]]='i';
    opcode_format[arr[12]]='i';
    opcode_format[arr[13]]='C';
    opcode_format[arr[14]]='C';
    opcode_format[arr[15]]='J';
    opcodes_mapping["add"]=arr[0];
    opcodes_mapping["addi"]=arr[1];
    opcodes_mapping["sub"]=arr[2];
    opcodes_mapping["subi"]=arr[3];
    opcodes_mapping["and"]=arr[4];
    opcodes_mapping["andi"]=arr[5];
    opcodes_mapping["or"]=arr[6];
    opcodes_mapping["ori"]=arr[7];
    opcodes_mapping["sll"]=arr[8];
    opcodes_mapping["srl"]=arr[9];
    opcodes_mapping["nor"]=arr[10];
    opcodes_mapping["sw"]=arr[11];
    opcodes_mapping["lw"]=arr[12];
    opcodes_mapping["beq"]=arr[13];
    opcodes_mapping["bneq"]=arr[14];
    opcodes_mapping["j"]=arr[15];
    sp_increment=opcodes_mapping["addi"];
    sp_decrement=registers_mapping["sp"];
    sp_increment=sp_increment<<16;
    sp_decrement=sp_decrement<<8;
    sp_increment=sp_increment|sp_decrement;
    sp_decrement=sp_decrement<<4;
    sp_increment=sp_increment|sp_decrement;
    sp_increment=sp_increment|1;
    sp_decrement=opcodes_mapping["subi"];
    sp_decrement=sp_decrement<<16;
    int temp=1<<16;
    temp--;
    temp=temp&sp_increment;
    sp_decrement=sp_decrement|temp;
    load=opcodes_mapping["lw"];
    store=opcodes_mapping["sw"];
    for(int i=0;i<9;i++)
    {
        hex_digits[i]='0'+i;
    }
    for(int i=10;i<16;i++)
    {
        hex_digits[i]='a'+(i-10);
    }
}

int constant(char f='\0')
{
    char c;
    int ans=0;
    bool neg=false;
    if(!((f=='-'||f=='+')||('0'<=f&&f<='9')))
    {
        while(scanf("%c",&f)!=EOF)
        {
            if(('0'<=f&&f<='9')||f=='-'||f=='+')
                break;
        }
    }
    if(f=='-'||f=='+')
    {
        neg=(f=='-');
        while(scanf("%c",&f)!=EOF)
        {
            if(('0'<=f&&f<='9'))
                break;
        }
    }
    if(('0'<=f&&f<='9'))
    {
        ans=(f-'0');
    }
    else return (1<<20);
    while(scanf("%c",&c)!=EOF)
    {
        if('0'<=c&&c<='9')
        {
            ans=ans*10+(c-'0');
            ans=ans&255;
        }
        else break;
    }
    if(neg) ans=-ans;
    ans=ans&255;
    return ans;
}
int register_input()
{
    string s("");
    char c='\0';
    while(scanf("%c",&c)!=EOF)
    {
        if(c=='$'||('0'<=c&&c<='9')||(c=='-'||c=='+'))
        {
            break;
        }
    }
    int ans=0;
    bool offset=false;
    if('0'<=c&&c<='9'||(c=='-'||c=='+'))
    {
        offset=(c=='-');
        ans=constant(c);
        while(scanf("%c",&c)!=EOF)
        {
            if(c=='$') break;
        }
    }
    ans=ans<<4;
    if(c!='$') return (1<<20);
    while(scanf("%c",&c)!=EOF)
    {
        if(!(('a'<=c&&c<='z')||('0'<=c&&c<='9'))) break;
        s=s+c;
    }
    if(registers_mapping.count(s))
    {
        int temp=registers_mapping[s];
        ans=ans|temp;
    }
    else return (1<<20);
    if(offset)
    {
        int curr=1<<12;
        ans=ans|curr;
    }
    return ans;
}

void output_binary(int instruction)
{
    for(int i=19; i>=0; i--)
    {
        int mask=1<<i;
        mask=mask&instruction;
        printf("%d",(mask>0));
    }
}

string label_opcode()
{
    string s("");
    char c='\0';
    while(scanf("%c",&c)!=EOF)
    {
        if((('A'<=c&&c<='Z')||('a'<=c&&c<='z')||('0'<=c&&c<='9')||(c=='_')))
        {
            s=s+c;
            break;
        }
    }
    while(scanf("%c",&c)!=EOF)
    {
        if(!(('A'<=c&&c<='Z')||('a'<=c&&c<='z')||('0'<=c&&c<='9')||(c=='_'))) break;
        if('A'<=c&&c<='Z') c=c+32;
        s=s+c;
    }
    return s;
}

int instructionC(int opcode)
{
    int dst=register_input();
    if(dst>=(1<<20)) return dst;
    int src=register_input();
    if(src>=(1<<20)) return src;
    dst=dst&15;
    dst=dst<<8;
    src=src&15;
    src=src<<12;
    string label=label_opcode();
    if(!labeling_number.count(label))
    {
        labeling_number[label]=labels++;
    }
    Q.push(make_pair(out.size(),labeling_number[label]));
    int out_num=opcode<<16;
    out_num=out_num|src|dst;
    return out_num;
}

int instructionI(int opcode,bool offset=false)
{
    int dst=register_input();
    if(dst>=(1<<20)) return dst;
    int src=register_input();
    if(src>=(1<<20)) return src;
    int address=0;
    if(offset)
    {
        address=src;
        address=address>>4;
        address=address&255;
    }
    else
    {
        address=constant();
    }
    if(address>=(1<<20)) return address;
    dst=dst&15;
    dst=dst<<8;
    src=src&15;
    src=src<<12;
    opcode=opcode<<16;
    opcode=opcode|src|dst|address;
    return opcode;
}

int instructionJ(int opcode)
{
    string label=label_opcode();
    if(!labeling_number.count(label))
    {
        labeling_number[label]=labels++;
    }
    Q.push(make_pair(out.size(),labeling_number[label]));
    int out_num=opcode<<16;
    return out_num;
}

int instructionR(int opcode,bool haveShift=false)
{
    int dst=register_input();
    if(dst>=(1<<20)) return dst;
    int src1=register_input();
    if(src1>=(1<<20)) return src1;
    int src2=src1;
    int shift=0;
    if(haveShift) shift=constant();
    else src2=register_input();
    if(src2>=(1<<20)) return src2;
    if(shift>=(1<<20)) return shift;
    dst=dst&15;
    dst=dst<<4;
    src1=src1&15;
    src1=src1<<12;
    src2=src2&15;
    src2=src2<<8;
    opcode=opcode<<16;
    shift=shift&15;
    opcode=opcode|src1|src2|dst|shift;
    return opcode;
}
int make_push()
{
    int reg=register_input();
    if(reg>=(1<<20)) return (1<<20);
    int hasOffset=1<<12;
    hasOffset=hasOffset&reg;
    int ans=0;
    if(hasOffset)
    {
        int opcode=load;
        opcode=opcode<<16;
        ans=ans|opcode;
        int freg=reg&15;
        freg=freg<<12;
        ans=ans|freg;
        int sreg=temp_reg;
        sreg=sreg<<8;
        ans=ans|sreg;
        int shift=reg>>4;
        shift=shift&255;
        ans=ans|shift;
        out.push_back(ans);

        ans=0;
        opcode=store;
        opcode=opcode<<16;
        ans=ans|opcode;
        freg=registers_mapping["sp"];
        freg=freg<<12;
        ans=ans|freg;
        ans=ans|sreg;
        out.push_back(ans);
        out.push_back(sp_decrement);
    }
    else
    {
        int opcode=store;
        opcode=opcode<<16;
        ans=ans|opcode;
        int freg=registers_mapping["sp"];
        freg=freg<<12;
        ans=ans|freg;
        int sreg=reg&15;
        sreg=sreg<<8;
        ans=ans|sreg;
        out.push_back(ans);
        out.push_back(sp_decrement);
    }
    return 0;
}
int make_pop()
{
    int reg=register_input();
    if(reg>=(1<<20)) return (1<<20);
    int hasOffset=1<<12;
    hasOffset=hasOffset&reg;
    int ans=0;
    if(hasOffset)
    {
        out.push_back(sp_increment);
        int opcode=load;
        opcode=opcode<<16;
        ans=ans|opcode;
        int freg=registers_mapping["sp"];
        freg=freg<<12;
        ans=ans|freg;
        int sreg=temp_reg;
        sreg=sreg<<8;
        ans=ans|sreg;
        out.push_back(ans);

        ans=0;
        opcode=store;
        opcode=opcode<<16;
        ans=ans|opcode;
        freg=reg&15;
        freg=freg<<12;
        ans=ans|freg;
        ans=ans|sreg;
        int shift=reg>>4;
        shift=shift&255;
        ans=ans|shift;
        out.push_back(ans);
    }
    else
    {
        out.push_back(sp_increment);
        int opcode=load;
        opcode=opcode<<16;
        ans=ans|opcode;
        int freg=registers_mapping["sp"];
        freg=freg<<12;
        ans=ans|freg;
        int sreg=reg&15;
        sreg=sreg<<8;
        ans=ans|sreg;
        out.push_back(ans);

    }
    return 0;
}
int read_code()
{
    stack_initialize();
    while(true)
    {
        string word=label_opcode();
        if(word.length()==0) break;
        if(opcodes_mapping.count(word))
        {
            int opcode=opcodes_mapping[word];
            int out_num;
            char opformat=opcode_format[opcode];
            if(opformat=='I'||opformat=='i')
            {
                out_num=instructionI(opcode,(opformat=='i'));
            }
            else if(opformat=='C')
            {
                out_num=instructionC(opcode);
            }
            else if(opformat=='J')
            {
                out_num=instructionJ(opcode);
            }
            else
            {
                out_num=instructionR(opcode,(opformat=='r'));
            }
            if(out_num>=(1<<20)) return out_num;
            out.push_back(out_num);
        }
        else if(word==Push)
        {
            int out_num=make_push();
            if(out_num>=(1<<20)) return (1<<20);
        }
        else if(word==Pop)
        {
            int out_num=make_pop();
            if(out_num>=(1<<20)) return (1<<20);
        }
        else
        {
            string label=word;
            if(!labeling_number.count(label))
            {
                labeling_number[label]=labels++;
            }
            label_position[labeling_number[label]]=out.size();
        }
    }
    while(!Q.empty())
    {
        pair<int,int> p=Q.front();
        Q.pop();
        int opcode=out[p.first];
        opcode=opcode>>16;
        char opformat=opcode_format[opcode];
        if(!label_position.count(p.second)) return (1<<20);
        int jump_address=label_position[p.second];
        if(opformat!='J')
        {
            jump_address=(jump_address-p.first-1)&255;
        }
        else
        {
            jump_address=jump_address&255;
            jump_address=jump_address<<8;
        }
        out[p.first]=out[p.first]|jump_address;
    }
    return 0;
}
int main()
{
    initialize_registers();
    initialize_opcodes();
    freopen("input.txt","r",stdin);
    int res=read_code();
    fclose(stdin);
    if(res==0)
    {
        freopen("output.hex","w",stdout);
        printf("v2.0 raw\n");
        for(int i=0; i<out.size(); i++)
        {
            //output_binary(out[i]);
            printf("%x",out[i]);
            printf(" ");
        }
        fclose(stdout);
    }
    else
    {
        printf("Error");
    }
    return 0;
}
