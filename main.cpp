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
   String_linked_lists*need;//需要的文件
   String_linked_lists*order;//生成这些文件的命令
   String_linked_lists*Dependent;//依赖的文件
   Required_documents*link = NULL;//链表指针
};
//存储文件和命令的对应关系
Required_documents *Required_Root = NULL;
//任务队列节点，需要执行的任务由多个链的节点连接而成
class Task_queues
{
    public:
    //执行编译
    String_linked_lists *need = NULL;//需要的文件
    Task_queues*D = NULL;//下一个节点，如果发现不能就地解决的话，构建传递给下一个自身实例
    //开始编译程序
    void run()
    {
        Required_documents *Required = Required_Root;
        //开始执行编译脚本
        std::cout<<"Start compilation"<<std::endl;

    }
};

//全局变量区
//根任务节点
Task_queues*Task_Root = NULL;

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
    //添加生成指令
    if(order.rfind("Dependent", 0) == 0)
    {
        std::cout<<"Add a build directive\n";
        //读取数据
        String_linked_lists*link = Bracket_handlers(order);
        //创建存储对象
        Required_documents *DATA = new Required_documents;
        if (link!=NULL)
        {
            //读取生成依赖项
            DATA->Dependent = commas_handlers(link->DATA);
            //删除节点
            String_linked_lists* i = link;
            link = link->link;
            delete i;//函数内new的对象，要在函数外delete
        }
        if (link!=NULL)
        {
            //读取生成指令
            DATA->order = commas_handlers(link->DATA);
            //删除节点
            String_linked_lists* i = link;
            link = link->link;
            delete i;//函数内new的对象，要在函数外delete
        }
        if (link!=NULL)
        {
            //读取生成文件
            DATA->need = commas_handlers(link->DATA);
            //删除节点
            String_linked_lists* i = link;
            link = link->link;
            delete i;//函数内new的对象，要在函数外delete
        }
        DATA->link = Required_Root;//加入链表
        Required_Root = DATA;
        //删除更多的节点
        while (link!=NULL)
        {
            String_linked_lists* i = link;
            link = link->link;
            delete i;//函数内new的对象，要在函数外delete
        }
        

    }
    //添加预处理器
    else if(order.rfind("replace", 0) == 0)
    {
        std::cout<<"Add a preprocessor\n";
        String_linked_lists*link = Bracket_handlers(order);
    }
    //添加目标文件
    else if(order.rfind("demand", 0) == 0)
    {
        std::cout<<"Add the target file\n";
        String_linked_lists*link = Bracket_handlers(order);
        if (link == NULL)
        {
            String_linked_lists **need = &Task_Root->need;//获取链表指针
            while ((*need)->link!=NULL)(*need)=(*need)->link;//移动到链表末尾
            (*need)->link = link;//插入链表
        }
        
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
    Task_Root->run();
}