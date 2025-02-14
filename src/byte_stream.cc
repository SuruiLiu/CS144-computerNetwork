#include "byte_stream.hh"
#include <vector>
#include <string_view>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : buffer_(capacity),capacity_( capacity ) {}

void Writer::push( string data )
{
  if (!write_open_) return;  // 如果写端已关闭，不允许写入

    // 可用容量
    uint64_t available = capacity_ - (nwrite_ - nread_);
    uint64_t to_push = std::min(static_cast<uint64_t>(data.size()), available);

    // 分两段写入（可能需要跨越缓冲区尾部）
    uint64_t first_chunk = std::min(to_push, capacity_ - (nwrite_ % capacity_));
    std::copy(data.begin(), data.begin() + first_chunk,
              buffer_.begin() + (nwrite_ % capacity_));

    if (to_push > first_chunk) {
      uint64_t second_chunk = to_push - first_chunk;
      std::copy(data.begin() + first_chunk, data.begin() + first_chunk + second_chunk,
                buffer_.begin());
    }

    nwrite_ += to_push;
    // 如果数据无法全部写入，你可以选择忽略剩余数据或设置 error 状态
}

void Writer::close()
{
  write_open_ = false;
  // Your code here.
}

bool Writer::is_closed() const
{
  return !write_open_; // Your code here.
}

uint64_t Writer::available_capacity() const
{
  return (capacity_ - (nwrite_ - nread_)); // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  return nwrite_; // Your code here.
}

string_view Reader::peek() const
{
  if (nwrite_ == nread_) return std::string_view(); // 缓冲区为空

  // 只返回一段连续数据，如果数据跨界则只返回到缓冲区末尾的那一段
  uint64_t index = nread_ % capacity_;
  uint64_t contiguous = std::min(capacity_ - index, nwrite_ - nread_);
  return std::string_view(buffer_.data() + index, contiguous); // Your code here.
}

void Reader::pop( uint64_t len )
{
  uint64_t available = nwrite_ - nread_;
  if (len > available) len = available;
  nread_ += len;; // Your code here.
}

bool Reader::is_finished() const
{
  return !write_open_ && (nwrite_ == nread_); // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return nwrite_ - nread_; // Your code here.
}

uint64_t Reader::bytes_popped() const
{
  return nread_; // Your code here.
}

