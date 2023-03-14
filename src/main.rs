// use nix::sys::socket::{socket,SockType};
// use nix::ioctl_read;


mod dev;


fn main() {


    loop{
        let  a = dev::parse_wireless_interfaces();

        print!("{}\r",a[0].q_link);

    }


    
    // // Create a UDP socket
    // let socket_fd = socket(
    //     nix::sys::socket::AddressFamily::Inet, 
    //     SockType::Datagram, 
    //     nix::sys::socket::SockFlag::empty(), 
    //     None, 
    // )
    // .expect("Failed to create socket");

}
