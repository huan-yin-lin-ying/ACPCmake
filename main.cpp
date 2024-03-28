#include <fstream>
#include <iostream>
//字符串链表，如果某个函数需要返回多个字符串的话就需要
struct String_linked_lists
{
   std::string DATA = "";//字符串
   String_linked_lists*link = NULL;//链表指针
};

//存储了需要的文件和生成它们的方法
struct Required_documents
{
   String_linked_lists need;//需要的文件
   String_linked_lists order;//生成这些文件的命令
   Required_documents*link;//链表指针
};
//任务队列节点，需要执行的任务由多个链的节点连接而成
class Task_queues
{
    public:
    Required_documents*Task;
};

//全局变量区
//根任务节点
Task_queues*Task_Root = NULL;

//引号处理程序，匹配第一个引号，并将其中的字符串提取出
void Draw_handlers()
{

}
//括号处理程序，匹配第一个括号，并将其中的字符串提取出
String_linked_lists *Bracket_handlers(std::string order)
{
    bool U = false;//是否进入写入点
    String_linked_lists *out = new String_linked_lists;
    for (size_t i = 0; i < order.length(); i++)
    {
        //检测是否为初始点
        if(order[i] == '(')U = true;
        //结束点
        else if(order[i] == ')')
        return out;
        //分隔符
        else if(order[i] == ',')
        {
            String_linked_lists* A = new String_linked_lists;
            A->link = out;
            out = A;
        }
        //插入数据
        else if(U) out->DATA+=order[i];
        else continue;
    }
    return out;
}
String_linked_lists *commas_handlers(std::string order)
{
    bool U = false;//是否进入写入点
    String_linked_lists *out = new String_linked_lists;
    for (size_t i = 0; i < order.length(); i++)
    {
        //检测是否为初始点
        if(order[i] == '{')U = true;
        //结束点
        else if(order[i] == '}')
        return out;
        //分隔符
        else if(order[i] == '|')
        {
            String_linked_lists* A = new String_linked_lists;
            A->link = out;
            out = A;
        }
        //插入数据
        else if(U) out->DATA+=order[i];
        else continue;
    }
    return out;
}
//指令解释器程序，由外部传入指令内部将指令转化为计算节点或程序配置
void Command_interpreter(std::string order)
{
    //依赖关系
    if(order.rfind("Dependent", 0) == 0)
    {
        std::cout<<"Add dependencies\n";
        Bracket_handlers(order);
    }
    //添加生成指令
    else if(order.rfind("Generation", 0) == 0)
    {
        std::cout<<"Add a build directive\n";
        Bracket_handlers(order);
    }
    //添加预处理器
    else if(order.rfind("replace", 0) == 0)
    {
        std::cout<<"Add a preprocessor\n";
        Bracket_handlers(order);
    }
    //添加目标文件
    else if(order.rfind("demand", 0) == 0)
    {
        std::cout<<"Add the target file\n";

        Bracket_handlers(order);
    }
}
//任务读取程序，用于从硬盘中读取任务
void Task_read()
{
    std::ifstream project("project.txt",std::ios::in);
    std::string TXT;
    while (project>>TXT)
    { 
        if(TXT.length() > 0 && TXT[0] == '#')
        {
            //std::cout<<"注释开始";
            do
            {
                //std::cout<<TXT;
                if (TXT.length() > 0 && TXT[TXT.length()-1] == '#')
                {
                    //std::cout<<"||注释结束"<<std::endl;
                    break;
                }
                
            }
            while (project>>TXT);
        }
        else
        {
            std::string order;
            do
            {
                order+=TXT;
                if (TXT.length() > 0 && TXT[TXT.length()-1] == ';')
                {
                    std::cout<<order<<std::endl;
                    Command_interpreter(order);
                    break;
                }
                order+=' ';
            }
            while (project>>TXT);
        }
    }
}
//主函数，程序入口，配置文件读取，初始化任务
int main()
{
    //system("chcp 65001");
    std::cout<<"The project.txt file is being read\n";
    Task_Root = new Task_queues;//new一个根对象
    Task_read();
    
}