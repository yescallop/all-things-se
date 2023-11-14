use std::{
    fmt::Debug,
    io::{self, BufRead, StdinLock},
    slice,
    str::{self, FromStr},
};

struct Scanner<'a> {
    stdin: StdinLock<'a>,
    buf: Vec<u8>,
    to_consume: usize,
}

#[allow(unused)]
impl<'a> Scanner<'a> {
    fn new(stdin: StdinLock<'a>) -> Self {
        Scanner {
            stdin,
            buf: vec![],
            to_consume: 0,
        }
    }

    fn next_str(&mut self) -> &str {
        self.skip_while(u8::is_ascii_whitespace).unwrap();
        let bytes = self.read_until(u8::is_ascii_whitespace).unwrap();
        return str::from_utf8(bytes).unwrap();
    }

    fn next<T: FromStr>(&mut self) -> T
    where
        T::Err: Debug,
    {
        self.next_str().parse().unwrap()
    }

    fn next_line_str(&mut self) -> &str {
        let bytes = self.read_until(|&b| b == b'\n').unwrap();
        return str::from_utf8(bytes).unwrap();
    }

    fn next_line<T: FromStr>(&mut self) -> T
    where
        T::Err: Debug,
    {
        self.next_line_str().parse().unwrap()
    }

    fn read_until(&mut self, mut pred: impl FnMut(&u8) -> bool) -> io::Result<&[u8]> {
        self.stdin.consume(self.to_consume);
        self.to_consume = 0;
        self.buf.clear();

        let mut read = false;
        loop {
            let available = match self.stdin.fill_buf() {
                Ok(n) if n.is_empty() => return Ok(&self.buf),
                Ok(n) => n,
                Err(ref e) if e.kind() == io::ErrorKind::Interrupted => continue,
                Err(e) => return Err(e),
            };
            if let Some(i) = available.iter().position(&mut pred) {
                self.to_consume = i + 1;
                if read {
                    self.buf.extend_from_slice(&available[..i]);
                    return Ok(&self.buf);
                }
                return Ok(unsafe { slice::from_raw_parts(available.as_ptr(), i) });
            }
            self.buf.extend_from_slice(available);
            let n = available.len();
            self.stdin.consume(n);
            read = true;
        }
    }

    fn skip_while(&mut self, mut pred: impl FnMut(&u8) -> bool) -> io::Result<()> {
        self.stdin.consume(self.to_consume);
        self.to_consume = 0;

        loop {
            let available = match self.stdin.fill_buf() {
                Ok(n) => n,
                Err(ref e) if e.kind() == io::ErrorKind::Interrupted => continue,
                Err(e) => return Err(e),
            };
            let n = available
                .iter()
                .position(|x| !pred(x))
                .unwrap_or(available.len());
            if n == 0 {
                return Ok(());
            }
            self.stdin.consume(n);
        }
    }
}
