int main() {
    int left = 0xff;
    int right = 0xff00;
    const int& sum = left + right;
    return 0;
}