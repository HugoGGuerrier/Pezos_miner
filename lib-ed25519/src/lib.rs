use ed25519_dalek::{Keypair, PublicKey, Signature, Signer, Verifier};
use libc;
use std::convert::TryFrom;
use std::os::raw::c_void;

#[no_mangle]
pub unsafe extern "C" fn ed25519_sign(
    out: *mut *mut u8,
    in_: *const u8,
    in_size: u64,
    private_key: *const u8,
    public_key: *const u8,
) -> i32 {
    // get keys as bytes buffers
    let pub_k: Vec<u8> = std::slice::from_raw_parts(public_key, 32).to_vec();
    let pri_k: Vec<u8> = std::slice::from_raw_parts(private_key, 32).to_vec();
    // println!("pub key : {:?}", pub_k);
    // println!("pri key : {:?}", pri_k);

    // make keypair
    let keys: Vec<u8> = [pri_k, pub_k].concat();
    let keypair = Keypair::from_bytes(&keys).unwrap();

    // get data as bytes buffer
    let data: Vec<u8> = std::slice::from_raw_parts(in_, in_size as usize).to_vec();

    // sign data
    let signature = keypair.sign(&data);
    let sig_bytes: Vec<u8> = signature
        .to_bytes().to_vec();
    // println!("signature (gen) : {:?}", sig_bytes);

    // return signed data
    libc::memcpy(*out as *mut c_void, sig_bytes.as_ptr() as *const c_void, 64);
    0
}

#[no_mangle]
pub unsafe extern "C" fn ed25519_verify(
    signature: *const u8,
    message:*const u8,
    msg_size: u64,
    public_key: *const u8,
) -> i32 {
    // get key and signature as bytes buffers
    let pub_k_buf: Vec<u8> = std::slice::from_raw_parts(public_key, 32).to_vec();
    let sig_buf: Vec<u8> = std::slice::from_raw_parts(signature, 64).to_vec();
    // println!("pub key : {:?}", pub_k_buf);
    // println!("sig : {:?}", sig_buf);

    // make key and signature as Rust objects
    let pub_k = PublicKey::from_bytes(&pub_k_buf).unwrap();
    let sig = Signature::new(<[u8; 64]>::try_from(sig_buf).unwrap());
    // println!("Pub Key (obj) : {:?}", pub_k);
    // println!("Sig (obj) : {:?}", sig);

    // get message as bytes buffer
    let msg: Vec<u8> = std::slice::from_raw_parts(message, msg_size as usize).to_vec();
    // println!("msg : {:?}", msg);

    // verify signature
    // println!("result : {:?}", pub_k.verify(&msg, &sig));
    match pub_k.verify(&msg, &sig) {
        Ok(_) => 1,
        Err(_) => 0,
    }
}
