#[allow(unused)]
#[derive(Debug)]
pub struct Communication {
    bytes: usize,
    packets: usize,
    errs: usize,
    drop: usize,
    fifo: usize,
    frame: usize,
    compressed: usize,
    multicast: usize,
}

#[allow(dead_code)]
#[derive(Debug)]
pub struct NetworkDevice {
    interface: String,
    receive: Communication,
    transmit: Communication,
}

impl Communication {
    fn new(
        bytes: usize,
        packets: usize,
        errs: usize,
        drop: usize,
        fifo: usize,
        frame: usize,
        compressed: usize,
        multicast: usize,
    ) -> Self {
        Self {
            bytes,
            packets,
            errs,
            drop,
            fifo,
            frame,
            compressed,
            multicast,
        }
    }
}

impl NetworkDevice {
    fn new(interface: String, receive: Communication, transmit: Communication) -> NetworkDevice {
        Self {
            interface,
            receive,
            transmit,
        }
    }
}

/*
The /proc/net/dev file in Linux provides statistics for all network interfaces on the system. Each line in the file represents one interface and contains detailed information about the traffic that has passed through the interface since the last reboot.

Here's a breakdown of the fields in /proc/net/dev:

    Interface name: The name of the network interface.

    Receive bytes: The total number of bytes received on this interface.

    Receive packets: The total number of packets received on this interface.

    Receive errors: The total number of packets received on this interface that contained errors.

    Receive drop: The total number of packets that were dropped because there was no space in the buffer.

    Receive FIFO: The total number of packets that were dropped from the input queue because of congestion.

    Receive frame: The total number of packets received on this interface that were larger than the MTU and were therefore truncated.

    Receive compressed: The total number of packets received on this interface that were compressed.

    Receive multicast: The total number of multicast packets received on this interface.

    Transmit bytes: The total number of bytes transmitted on this interface.

    Transmit packets: The total number of packets transmitted on this interface.

    Transmit errors: The total number of packets transmitted on this interface that contained errors.

    Transmit drop: The total number of packets that were dropped because there was no space in the buffer.

    Transmit FIFO: The total number of packets that were dropped from the output queue because of congestion.

    Transmit frame: The total number of packets transmitted on this interface that were larger than the MTU and were therefore truncated.

    Transmit compressed: The total number of packets transmitted on this interface that were compressed.

    Transmit multicast: The total number of multicast packets transmitted on this interface.

The receive and transmit statistics are separate and provide information about the amount of traffic that has been sent and received on the interface since the last reboot. The error and drop statistics provide information about the quality of the connection and can help identify potential problems with the network interface.

Note that the values in /proc/net/dev are cumulative since the last reboot, so if you want to get statistics for a specific time period, you will need to subtract the values obtained at the beginning of the period from the values obtained at the end of the period.
 */
pub fn mai() -> Vec<NetworkDevice> {
    let contents = std::fs::read_to_string("/proc/net/dev").expect("Error reading file");

    contents
        .lines()
        .skip(2)
        .map(|line| {
            let mut split = line.split_whitespace();
            let mut interface = split.next().unwrap().to_string();
            interface.pop();

            NetworkDevice {
                interface: interface,
                receive: Communication::new(
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                ),
                transmit: Communication::new(
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                    split.next().unwrap().parse().unwrap(),
                ),
            }
        })
        .collect()
}

#[derive(Debug)]
pub struct WirelessInterface {
    pub interface: String,
    pub status: f64,
    pub q_link: f64,
    pub level: f64,
    pub noise: f64
}

pub fn parse_wireless_interfaces() -> Vec<WirelessInterface> {

    let contents = std::fs::read_to_string("/proc/net/wireless").expect("Error reading file");
    let mut interfaces = vec![];

    for line in contents.lines().skip(2) {
        let mut parts = line.split_whitespace();
        let interface = parts.next().unwrap().trim_matches(':').to_string();
        let status = parts.next().unwrap().trim_matches('"').parse::<f64>().unwrap();
        let q_link = parts.next().unwrap().parse::<f64>().unwrap();
        let level = parts.next().unwrap().parse::<f64>().unwrap();
        let noise = parts.next().unwrap().parse::<f64>().unwrap();

        interfaces.push(WirelessInterface {
            interface,
            status,
            q_link,
            level,
            noise
        });
    }

    interfaces
}
