# AoC 2018 Notes

### Day 11

暴力累加方法在M1 Max芯片上大约1-2分钟。有个简单的加速方法是计算x和y方向的prefix sum，然后每次增加z的时候只需要加x段和y段（要减去一个右下角重复的值。太懒了没有实现。

### Day 10

这是个脑筋急转弯的题，没有太大意思。跳过。

### Day 9

这是个循环链表的基本操作问题。我用C风格实现了一个循环链表，然后包装在一个C++类里面方便操作。和之前的问题一样，这里同样忽略了CircularList对象析构时释放内存的问题。

*看Norvig的解法，似乎Python的deque是一种循环连续存储结构，支持rotate操作。但是C++的deque不支持rotate。*

### Day 8

基本的循环构造和遍历树形结构的问题。写了一个C风格的实现，偷懒最后没有释放内存。

### Day 7

基本的topsort算法在问题中已经描述了；第二部分针对时间做了一个循环（参考到答案时间不是太大），简化实现。