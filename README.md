# Base64
A simple base 64 string encoder/decoder, released into the public domain. This works on all C++ standards, from 98 through to 20, and is unlikely to be broken by any future standard. It has no dependencies outside of the standard library, and only uses the `<string>` header from that. It is fairly performant - it would be possible, and fairly trivial, to optimize it further using tools from later standards (such as `std::string_view` and some compile-time programming) but this would compromise broad compatibility, and simplicity of the code.

I recently went looking for a pre-built solution to this problem, and most of the pre-built solutions I could find either used questionable techniques, had dependencies which didn't fit with the greater project I was working on, or had licensing strings attached. Nothing wrong with the latter two of those of course, but I figured I'd provide a simple, easy, license-free, drop-in solution.

I'm aware that people looking for drop-in solutions are often divided between preferring a header-only solution for simple dropping-in, and wanting a traditional header-and-cpp combination for better compiling. I offer both. The code in both solutions is equivalent, so take your pick. I make no claim that the functions included here are the best candidates for a header-only, `inline` approach, but it will still work nicely.



## Notes on the code
I make no secret of the fact this code was adapted from another public domain approach found on [WikiBooks](https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C++), however that code came with the baggage of requiring Windows-specific preprocessor `#define`s, and made a few code decisions which I felt were perhaps suboptimal.

Some adaptation may be needed if you are using an uncommon implementation or character set, but this code was written to be simple to use, simple to understand, and easy to change and adapt as needed. 
