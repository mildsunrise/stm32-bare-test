use std::env;
use std::path::Path;
use std::fs::File;
use std::process::Command;

fn main() {
    assert_eq!(env::var("TARGET").unwrap(), "thumbv7em-none-eabi");

    let out_dir = env::var("OUT_DIR").unwrap();

    let constants_src = "src/memory.rs";
    let constants_out = Path::new(&out_dir).join("constants.h");
    let linker_src = "code.lds";
    let linker_out = Path::new(&out_dir).join("code.lds");

    // generate constants for use in linker script

    let mut config = cbindgen::Config::default();
    config.language = cbindgen::Language::C;
    config.no_includes = true;
    config.export.item_types = vec![cbindgen::ItemType::Constants];

    println!("cargo:rerun-if-changed={}", constants_src);
    cbindgen::Builder::new()
      .with_src(constants_src)
      .with_config(config)
      .generate().expect("failed to generate constants")
      .write_to_file(constants_out);

    // generate linker script (preprocess with constants)

    println!("cargo:rerun-if-changed={}", linker_src);
    let input = File::open(linker_src.clone())
      .expect("failed to read input linker script");
    let output = File::create(linker_out.clone())
      .expect("failed to create output linker script");

    let status = Command::new("cpp").arg("-P")
      .args(["-I", &out_dir])
      .stdin(input).stdout(output)
      .status().expect("failed to execute preprocessor, is cpp installed?");
    assert!(status.success(), "failed to preprocess linker script");

    // use it when linking
    println!("cargo:rustc-link-arg=-T{}", linker_out.to_str().unwrap());
}
