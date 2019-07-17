//https://coolshell.cn/articles/1443.html
#include <string>
#include <iostream>
#include <sys/time.h>
using namespace std;
static long getcurrenttick()
{
    long tick ;
    struct timeval time_val;
    gettimeofday(&time_val , NULL);
    tick = time_val.tv_sec * 1000 + time_val.tv_usec / 1000 ;
    return tick;
}
 
 /*first without get value*/
//short
int main( )
{
    string the_base(1024 * 1024 * 10, 'x');
    long begin =  getcurrenttick();
    for( int i = 0 ;i< 100 ;++i ) {
       string the_copy = the_base ;
    }
    fprintf(stdout,"耗时[%d] \n",getcurrenttick() - begin );
}

//4s 因为有写操作
/*
int main()
{
string the_base(1024*1024*10,'x');
long begin = getcurrenttick();
for(int i=0;i<100;i++)
{
string the_copy = the_base;
the_copy[0] = 'y';//写操作
}

fprintf(stdout,"耗时[%d] \n",getcurrenttick() - begin );
return 0;
}*/


//4s
/*
以下代码
啊，居然也是4~5秒！你可能在想，我只是做了一个读，没有写嘛，这到底是怎么回事？难道还有读时也拷贝的技术！。

不错，为了避免了你通过[]操作符获取string内部指针而直接修改字符串的内容，在你使用了the_base[0]后，这个字符串的写时才拷贝技术就失效了。

C++标准的确就是这样的，C++标准认为，当你通过迭代器或[]获取到string的内部地址的时候，string并不知道你将是要读还是要写。这是它无法确定，为此，当你获取到内部引用后，为了避免不能捕获你的写操作，它在此时废止了写时才拷贝技术！

这样看来我们在使用COW的时候，一定要注意，如果你不需要对string的内部进行修改，那你就千万不要使用通过[]操作符和迭代器去获取字符串的内部地址引用，如果你一定要这么做，那么你就必须要付出代价。当然，string还提供了一些使迭代器和引用失效的方法。比如说push_back，等， 你在使用[]之后再使用迭代器之后，引用就有可能失效了。那么你又回到了COW的世界！比如下面的一个例子*/
int main(void) {
    string the_base(1024 * 1024 * 10, 'x');
    fprintf(stdout,"the_base's first char is [%c]\n",the_base[0] );//有取值操作
    long begin =  getcurrenttick();
    for (int i = 0; i < 100; i++) {
        string the_copy = the_base;
    }
    fprintf(stdout,"耗时[%d] \n",getcurrenttick() - begin );
}


//short
//使用push_back之后，之前的[]取值操作得到的引用就失效了，速度又上去了


int main( )
{
    struct timeval time_val;
    string the_base(1024 * 1024 * 10, 'x');
    long begin = 0 ;
    fprintf(stdout,"the_base's first char is [%c]\n",the_base[0] );
    the_base.push_back('y');//push_back使之前的[]取值失效
    begin = getcurrenttick();
    for( int i = 0 ;i< 100 ;++i ) {
        string the_copy = the_base ;
    }
    fprintf(stdout,"耗时[%d] \n",getcurrenttick() - begin );
}



/*
string original = "hello";
char & ref = original[0];
string clone = original;
ref = 'y';

我们生成了一个string，并保留了它首字符的引用，然后复制这个string，修改string中的首字符。因为写操作只是直接的修改了内存中的指定位置，这个string就根本不能感知到有写发生，如果写时才拷贝是不成熟的，那么我们将同时会修改original和clone两个string。那岂不是灾难性的结果？幸好上述问题不会发生。clone的值肯定是没有被修改的。看来COW就是非常的牛！
*/
以上都证明了我们的COW技术非常牛！
