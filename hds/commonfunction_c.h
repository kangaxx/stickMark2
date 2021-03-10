#ifndef COMMONFUNCTION_C_H
#define COMMONFUNCTION_C_H
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#include <string>
#include <assert.h>
#include "common.h"

using namespace std;
#ifndef INT_MAKE_PATCH_SERIALNO
#define INT_MAKE_PATCH_SERIALNO "1.05.0000"
#endif

namespace commonfunction_c {

class BaseFunctions
{
public:
    enum eFindPathMode{FPM_ALL=0007,FPM_SYSENV=0001,FPM_PATHWORK=0002,FPM_PATHSTR=0004};
    BaseFunctions(){m_FileName = ""; m_FilePath = "";}
    BaseFunctions(string fileName, string filePath=""):m_FileName(fileName),m_FilePath(filePath){}
    ~BaseFunctions(){}
    static int GetWorkPath(char *dest); //get program run time path info!
    static const char* getConfigPath(int mode=FPM_ALL); //mode : FPM_ALL,exec in the following order: SYSENV,PATHSTR,PATHWORK
    static string GetParaByName(string fileName, string name);//work for new type ini,ex: "password=123456";
    static string GetParaByName_safe(string fileName, string name);
    static string GetParaByLine(string fileName, int lineNum);//work for old type,just have value ex : "123456"

    static char *Int2Chars(int in, char *out, int size=DEFAULT_INTCHAR_SIZE);
    static int Chars2Int(const char *in, int size=DEFAULT_INTCHAR_SIZE);
    static int Str2Int(string s);
    static int Str2Int(string s, int i);
    static string time2str(time_t time);//transfer time 2 string
    static time_t str2time(string str); //transfer string 2 time,from '1970-01-01 00:08:00'
    //将日期转换位各种格式的int值，暂时只支持年加日（天数），后续增强各种功能。
    static int time2int(time_t time, string ="yyddd"); //transfer time 2 int
    static string cutValue(string wholeStr,string regExp); //find regExp in wholeStr, in regExp, word "value" is the return,
    //exp: reduce_type|value|,will get "aaa" in whole string "reduce_type|aaa|"
    static string combineFilePath(string path,string fileName); //combine file name and path name , will confirm if there is an '/' between path and file;
    static string combineFilePath_shout(string path,string fileName); //combine file , if file not access , will throw error
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //getProcArgv: to get process argument, set *title to argu name ,like "-l" ,
    //             it will try to match three mode successively, "-labc" , "-l=abc", "-l abc -other" ,
    //             if matched , result is true , and dest will set to abc.
    //             specially , "ls -l" or "ls -l -abc", still return true , dest set to '\0';
    //             dest could be NULL if no or not need value;
    static bool getProcArgv(int argc,const char *argv[],const char *title, char *dest);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief getSystemLogPath
    /// \param dest
    /// \param subPath
    ///  get QModeling's system log file path , base on env("QM_ROOT"), return path or null(fail, no env or other)
    static char *getSystemLogPath(char *dest);
    static char *getSystemIconPath(char *dest);
    //根据参数计算出软件版权期限
    static int getLicenseLimitDay(int dayBegin, int dayNum);


private:
    string m_FileName,m_FilePath;

    static char *getSysEnvConfigPath(char *dest); //get system env for path , if not found env or path , rutern NULL;
    static int freadLine(void *buffer, int size, FILE *f);
    static int strcmp_nonsafe(const char *c1, const char *c2); // result 0  : c1 == c2 , 1 : c2 include c1 , -1 : c2 not include c1
};

typedef struct ShmInitials
{
    const char *path;// for linux sys/shm.h function now
    int ipcKey; //for linux sys/ipc.h ftok now
    int size;   //the share memory's capcity chars num;
}stc_ShmInitials;

//桥接模式设计下的共享内存管理模组，将LINUX或其他系统不同的共享内存行为包装成相同的上层行为，供用户使用
template<class T>
class IShareMemImp
{
public:
    IShareMemImp(){}
    IShareMemImp(stc_ShmInitials &i) {}
    virtual ~IShareMemImp() = 0;
    virtual void Initial(stc_ShmInitials &i) = 0;
    virtual int CreateShmId() = 0; //create share memory and return sharemem id , which return by system function shmget,return -1 means error;
    virtual int GetShmId() = 0; //return the created share memory id(linux system ipc num),return -1 means error;
    virtual T *GetShmAddr() = 0; //Link share memory address point to link local point,return NULL Point means error;
    virtual int DtShm(T *t) = 0; //for linux shm ,it's detach local memory,not clear share memory ,return -1 means error;
    virtual int ClearShm() = 0; //for linux shm ,it's destory share memory both system and local;
};

template<class T>
class ShareMemImp_Linux:public IShareMemImp<T>
{
public:
    ShareMemImp_Linux(){;}
    ShareMemImp_Linux(stc_ShmInitials &i):m_Initial(i){}
    ~ShareMemImp_Linux();
    inline void Initial(stc_ShmInitials &i){m_Initial = i;}
    inline int CreateShmId(){
        //开辟共享内存空间
        int shm_id = shmget(GetKey(),m_Initial.size,IPC_CREAT|IPC_EXCL|0666);
        return shm_id;
    }
    inline int GetShmId(){
        int shm_id = shmget(GetKey(),0,0);
        return shm_id;
    }
    inline T *GetShmAddr(){
        T *result;
        int id = GetShmId();
        if (id == ERROR_SYSSHM_NORMAL)
            return NULL;
        result = (T*)shmat(id,NULL,0);
        return result;
    }
    inline int DtShm(T *t){
        if (t == NULL)
            return ERROR_SYSSHM_NORMAL;
        return shmdt(t);
    }
    inline int ClearShm(){
        int temp = GetShmId();
        if (temp == ERROR_SYSSHM_NORMAL)
            return ERROR_SYSSHM_NORMAL;
        return shmctl(temp,IPC_RMID,NULL);
    }

private:
    stc_ShmInitials m_Initial;
    inline int GetKey(){
        return ftok(this->m_Initial.path,this->m_Initial.ipcKey); //comment tempory
    }

};

template<class T>
class IShareMemAbstraction
{
public:
    virtual ~IShareMemAbstraction()=0;
    virtual int CreateShmId() = 0; //create share memory ,call imp's operation
    virtual int GetShmId() = 0; //call imp's operation
    virtual T *GetShmAddr() = 0; //call imp's operation
    virtual int DtShm(T *t) = 0; //call imp's operation
    virtual int ClearShm() = 0; //call imp's operation
};



template<class T>
class IShareMemDerive:public IShareMemAbstraction<T>
{
public:
    IShareMemDerive(IShareMemImp<T> &imp):m_Imp(&imp){}
    ~IShareMemDerive();
    inline int CreateShmId(){return m_Imp->CreateShmId();}
    inline int GetShmId(){return m_Imp->GetShmId();} //call imp's operation
    inline T *GetShmAddr(){return m_Imp->GetShmAddr();} //call imp's operation
    inline int DtShm(T *t){return m_Imp->DtShm(t);} //call imp's operation
    inline int ClearShm(){return m_Imp->ClearShm();} //call imp's operation
private:
    IShareMemImp<T> *m_Imp;
};

template<class T>
IShareMemAbstraction<T>::~IShareMemAbstraction()
{
    //nothing yet
}

template<class T>
IShareMemDerive<T>::~IShareMemDerive()
{
    //nothing yet
}

template<class T>
IShareMemImp<T>::~IShareMemImp()
{
    //nothing yet
}

template<class T>
ShareMemImp_Linux<T>::~ShareMemImp_Linux()
{
    //nothing yet
}

//环形双链表，存储数据高效，在无法确认数组长度时，链表无需反复申请释放大块内存的优势较明显。
template<class T>
class DuLink{
private:
    class Node{
    public:
        T data;
        Node *prior;
        Node *next;
        Node(T &element,Node *prior, Node *next):data(element){this->next = next;this->prior = prior; prior->next=next->prior = this;}
        Node():data(data){}
        ~Node(){prior->next = next; next->prior = prior;}
    };
    Node *head;
    Node *current;
public:
    DuLink():head(new Node()){head->prior=head->next=head; current = NULL;}
    int size(){
        int count = 0;
        for (Node *p=head->next;p!= head;p=p->next)
            count ++;
        return count;
    }

    T &operator [](int idx){return getElement(idx);}
    //判断是否为空链
    bool isEmpty()const{return head==head->next?true:false;}
    //将元素添加至最后，注意node的指针设置
    void addToLast(const T& element){Node* ne=new Node(element,head->prior,head);}
    //获取最后一个元素
    T getLastElement()const{assert(!isEmpty());return head->prior->data;}
    //删除最后一个元素，注意node的指针设置
    void delLastElement(){assert(!isEmpty());Node* p=head->prior;delete p;}
    //修改最后一个元素
    void alterLastElement(const T& newElement){assert(!isEmpty());head->prior->data=newElement;}
    //插入元素
    void insert(T element, int pos){insertElement(element, pos);}
    void insertElement(T element, int pos){
        assert(pos >= 0);
        assert(pos < size());
        Node *p = head;
        while(pos-- != 0)
            p=p->next;
        new Node(element,p->prior,p);
    }

    //获取元素
    T &get(int idx){return getElement(idx);}
    T &getElement(int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *p = head->next;
        while(idx-- != 0)
            p=p->next;
        return p->data;
    }
    //删除元素
    T del(int idx){return delElement(idx);}
    T delElement(int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *t = head->next;
        while(idx-- != 0)
            t = t->next;
        T ret = t->data;
        delete t;
        return ret;
    }

    //修改元素
    void alter(const T &newElement, int idx){alterElement(newElement,idx);}
    void alterElement(const T &newElement,int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *t = head->next;
        while (idx-- != 0)
            t= t->next;
        t->data = newElement;
        return;
    }

    //查找元素 ， 返回值小于0表示未找到元素
    int findElement(const T& element) const{
        char *ori, *in;
        int result = 0;
        Node *t = head->next;
        while (t != head){
            ori = reinterpret_cast<char *>(&(t->data));
            in = reinterpret_cast<char *>(&element);
            if (strncmp(ori,in,sizeof(T)) == 0)
                return result;
            result++;
            t= t->next;
        }
        return -1;
    }
    //正序遍历
    void Traverse(void (*visit)(T&element)){
        Node *t = head->next;
        while(t != head){
            visit(t->data);
            t = t->next;
        }
    }

    //逆序遍历
    void TraverseBack(void (*visit)(T&element)){
        Node *t = head->prior;
        while (t != head){
            visit(t->data);
            t= t->prior;
        }
    }

    //清空链表
    void clear(){clearAllElement();}
    void clearAllElement(){
        Node *t,*p;
        p = head->next;
        while(p != head){
            t = p->next;
            delete p;
            p = t;
        }
    }

    //游标操作
    int getCurrentIdx(){
        assert(size() > 0);
        assert(head != head->next);
        int result = 0;
        if (current == NULL || current == head)
            return 0;
        else{
            Node *t = head->next;
            while (current != t){
                result++;
                t = t->next;
            }
        }
    }

    T& getCurrent(){
        assert(size() > 0);
        assert(head != head->next);
        if (current == NULL || current == head)
            current = head->next;
        return current->data;
    }
    T& goNext(){
        assert(size() > 0);
        assert(head != head->next);
        getCurrent();
        Node *p = current->next;
        if (p == head)
            p = p->next;
        current = p;
        return current->data;
    }

    T& goPrior(){
        assert(size() > 0);
        assert(head != head->prior);
        getCurrent();
        Node *p = current->prior;
        if (p == head)
            p = p->prior;
        current = p;
        return current->data;
    }

    //销毁链表
    ~DuLink(){
        clearAllElement();
        if (head != NULL){
            delete head;
            head = NULL;
        }
    }
};

//特化模板
//环形双链表，存储数据高效，在无法确认数组长度时，链表无需反复申请释放大块内存的优势较明显。
template<class T>
class DuLink<T*>
{
private:
    class Node{
    public:
        const T *data;
        Node *prior;
        Node *next;
        Node(const T *element,Node *prior, Node *next):data(element){this->next = next;this->prior = prior; prior->next=next->prior = this;}
        Node():data(NULL){}
        ~Node(){prior->next = next; next->prior = prior;}
    };
    Node *head;
    Node *current;
public:
    DuLink():head(new Node()){head->prior=head->next=head; current = NULL;}
    int size(){
        int count = 0;
        for (Node *p=head->next;p!= head;p=p->next)
            count ++;
        return count;
    }

    const T *operator [](int idx){return getElement(idx);}
    //判断是否为空链
    bool isEmpty()const{return head==head->next?true:false;}
    //将元素添加至最后，注意node的指针设置
    void addToLast(const T* element){new Node(element,head->prior,head);}
    //获取最后一个元素
    const T *getLastElement()const{assert(!isEmpty());return head->prior->data;}
    //删除最后一个元素，注意node的指针设置
    void delLastElement(){assert(!isEmpty());Node* p=head->prior;delete p;}
    //修改最后一个元素
    void alterLastEmlent(const T* newElement){assert(!isEmpty());head->prior->data=newElement;}
    //插入元素
    void insert(T element, int pos){insertElement(element, pos);}
    void insertElement(T *element, int pos){
        assert(pos >= 0);
        assert(pos < size());
        Node *p = head;
        while(pos-- != 0)
            p=p->next;
        new Node(element,p->prior,p);
    }

    //获取元素
    const T *get(int idx){return getElement(idx);}
    const T *getElement(int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *p = head->next;
        while(idx-- != 0)
            p=p->next;
        return p->data;
    }
    //删除元素
    const T *del(int idx){return delElement(idx);}
    const T *delElement(int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *t = head->next;
        while(idx-- != 0)
            t = t->next;
        const T *ret = t->data;
        delete t;
        return ret;
    }

    //修改元素
    void alter(const T *newElement, int idx){alterElement(newElement,idx);}
    void alterElement(const T *newElement,int idx){
        assert(idx >= 0);
        assert(idx < size());
        Node *t = head->next;
        while (idx-- != 0)
            t= t->next;
        t->data = newElement;
        return;
    }

    //查找元素 ， 返回值小于0表示未找到元素
    int findElement(const T* element) const{
        char *ori, *in;
        int result = 0;
        Node *t = head->next;
        while (t != head){
            ori = reinterpret_cast<char *>(&(t->data));
            in = reinterpret_cast<char *>(&element);
            if (strncmp(ori,in,sizeof(T)) == 0)
                return result;
            result++;
            t= t->next;
        }
        return -1;
    }
    //正序遍历
    void Traverse(void (*visit)(T* element)){
        Node *t = head->next;
        while(t != head){
            visit(t->data);
            t = t->next;
        }
    }

    //逆序遍历
    void TraverseBack(void (*visit)(T* element)){
        Node *t = head->prior;
        while (t != head){
            visit(t->data);
            t= t->prior;
        }
    }

    //清空链表
    void clear(){clearAllElement();}
    void clearAllElement(){
        Node *t,*p;
        p = head->next;
        while(p != head){
            t = p->next;
            delete p;
            p = t;
        }
    }

    //游标操作
    int getCurrentIdx(){
        assert(size() > 0);
        assert(head != head->next);
        int result = 0;
        if (current == NULL || current == head)
            return 0;
        else{
            Node *t = head->next;
            while (current != t){
                result++;
                t = t->next;
            }
        }
        return result;
    }

    const T* getCurrent(){
        assert(size() > 0);
        assert(head != head->next);
        if (current == NULL || current == head)
            current = head->next;
        return current->data;
    }
    const T* goNext(){
        assert(size() > 0);
        assert(head != head->next);
        getCurrent();
        Node *p = current->next;
        if (p == head)
            p = p->next;
        current = p;
        return current->data;
    }

    const T* goPrior(){
        assert(size() > 0);
        assert(head != head->prior);
        getCurrent();
        Node *p = current->prior;
        if (p == head)
            p = p->prior;
        current = p;
        return current->data;
    }

    //销毁链表
    ~DuLink(){
        clearAllElement();
        if (head != NULL){
            delete head;
            head = NULL;
        }
    }
};

///////////////////////////////////////////////////////////////////////////////////
//    TO COMPILE QfDebug, please add make patch num to .pro file, e.x:           //
//      DEFINES += "INT_MAKE_PATCH_SERIALNO=1.0f"                                //
//      QfDebug won't work in release mode(it will do nothing)                   //
//    qfDebug will print input info if arg 'PatchNum' bigger than DEFINES        //
///////////////////////////////////////////////////////////////////////////////////

template<class T> void qfDebug(T info,const char *patchNum)
{
    if (strcmp(patchNum , INT_MAKE_PATCH_SERIALNO) >= 0)
        cout << "Debug info : " << info  << " (patch no " << patchNum << ")" << endl;
}


template<> void qfDebug<const char*>(const char* info,const char *PatchNum);


}


#endif // COMMONFUNCTION_C_H
