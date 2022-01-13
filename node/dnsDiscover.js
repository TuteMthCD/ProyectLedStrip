const mDnsSd = require('node-dns-sd');

mDnsSd.discover({
  name: '_esp._tcp.local'
}).then((device_list) => {
  console.log(device_list[0].address.toString());
  console.log(device_list[0].fqdn.toString());
}).catch((error) => {
  console.error(error);
});
// mDnsSd.ondata = (packet) => {
//   console.log(JSON.stringify(packet, null, '  '));
// };

// mDnsSd.startMonitoring().then(() => {
//   console.log('Started.');
// }).catch((error) => {
//   console.error(error);
// });