use std::{
    fmt::Debug,
    io::{self, BufRead},
    slice,
    str::{self, FromStr},
};

struct Interval {
    start: u32,
    end: u32,
    efficiency: u32,
}

fn main() {
    let stdin = io::stdin();
    let mut s = Scanner::new(stdin.lock());

    let _n: u32 = s.next();
    let m: usize = s.next();
    let r: u32 = s.next();

    let mut intervals: Vec<_> = (0..m)
        .map(|_| Interval {
            start: s.next(),
            end: s.next::<u32>() + r,
            efficiency: s.next(),
        })
        .collect();
    intervals.sort_unstable_by_key(|x| x.end);

    let last_available: Vec<_> = intervals
        .iter()
        .enumerate()
        .map(|(i, it)| intervals[..i].partition_point(|x| x.end <= it.start))
        .collect();

    let mut sol = vec![0; m + 1];
    for i in 0..m {
        sol[i + 1] = sol[i].max(sol[last_available[i]] + intervals[i].efficiency);
    }
    println!("{}", sol[m]);
}

struct Scanner<R> {
    inner: R,
    buf: Vec<u8>,
    to_consume: usize,
}

#[allow(unused)]
impl<R: BufRead> Scanner<R> {
    fn new(inner: R) -> Self {
        Scanner {
            inner,
            buf: vec![],
            to_consume: 0,
        }
    }

    fn next_str(&mut self) -> &str {
        self.skip_while(u8::is_ascii_whitespace).unwrap();
        let bytes = self.read_until(u8::is_ascii_whitespace).unwrap();
        str::from_utf8(bytes).unwrap()
    }

    fn next<T: FromStr>(&mut self) -> T
    where
        T::Err: Debug,
    {
        self.next_str().parse().unwrap()
    }

    fn next_line_str(&mut self) -> &str {
        let bytes = self.read_until(|&b| b == b'\n').unwrap();
        str::from_utf8(bytes).unwrap()
    }

    fn next_line<T: FromStr>(&mut self) -> T
    where
        T::Err: Debug,
    {
        self.next_line_str().parse().unwrap()
    }

    fn read_until(&mut self, mut pred: impl FnMut(&u8) -> bool) -> io::Result<&[u8]> {
        self.inner.consume(self.to_consume);
        self.to_consume = 0;
        self.buf.clear();

        let mut read = false;
        loop {
            let available = match self.inner.fill_buf() {
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
            self.inner.consume(n);
            read = true;
        }
    }

    fn skip_while(&mut self, mut pred: impl FnMut(&u8) -> bool) -> io::Result<()> {
        self.inner.consume(self.to_consume);
        self.to_consume = 0;

        loop {
            let available = match self.inner.fill_buf() {
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
            self.inner.consume(n);
        }
    }
}
