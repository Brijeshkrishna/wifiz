// // use nix::sys::socket::{socket,SockType};
// // use nix::ioctl_read;

// use libc::ioctl;

// mod dev;
// use std::io;
// use libc;

// fn main()  {



//     // // Create a UDP socket
//     // let socket_fd = socket(
//     //     nix::sys::socket::AddressFamily::Inet,
//     //     SockType::Datagram,
//     //     nix::sys::socket::SockFlag::empty(),
//     //     None,
//     // )
//     // .expect("Failed to create socket");

// }

#[link(name = "a")]
extern "C" {
        fn wifi();
    }
    
    fn main() {
        let result = unsafe { wifi() };
    }
    


