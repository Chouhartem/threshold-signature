# Threshold signatures

This is a proof of concept implementation of the practical signature from [Born and Raised Distributively: Fully Distributed Non-Interactive Adaptively-Secure Threshold Signatures with Short Shares](https://hal.inria.fr/hal-00983149) from Benoît Libert, Marc Joye and Moti Yung.

This C++ implementation relies on the [Relic](https://github.com/relic-toolkit) toolkit from Diego Aranha.

**Usage:** ./main t n Msg

* `t` is the minimum number of users needed for the reconstruction
* `n` the total number of users
* `Msg` is the message to be signed
