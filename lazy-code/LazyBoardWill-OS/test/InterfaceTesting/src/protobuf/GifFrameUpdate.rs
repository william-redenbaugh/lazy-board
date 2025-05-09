// Automatically generated rust module for 'GifFrameUpdate.proto' file

#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(unused_imports)]
#![allow(unknown_lints)]
#![allow(clippy::all)]
#![cfg_attr(rustfmt, rustfmt_skip)]


use quick_protobuf::{MessageRead, MessageWrite, BytesReader, Writer, WriterBackend, Result};
use quick_protobuf::sizeofs::*;
use super::*;

#[derive(Debug, Default, PartialEq, Clone)]
pub struct GIFMessage {
    pub x_size: i32,
    pub y_size: i32,
    pub color_space: mod_GIFMessage::ColorSpace,
    pub message_size: i32,
}

impl<'a> MessageRead<'a> for GIFMessage {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.x_size = r.read_int32(bytes)?,
                Ok(16) => msg.y_size = r.read_int32(bytes)?,
                Ok(24) => msg.color_space = r.read_enum(bytes)?,
                Ok(32) => msg.message_size = r.read_int32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl MessageWrite for GIFMessage {
    fn get_size(&self) -> usize {
        0
        + if self.x_size == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.x_size) as u64) }
        + if self.y_size == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.y_size) as u64) }
        + if self.color_space == GifFrameUpdate::mod_GIFMessage::ColorSpace::BIT_24_RGB { 0 } else { 1 + sizeof_varint(*(&self.color_space) as u64) }
        + if self.message_size == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.message_size) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.x_size != 0i32 { w.write_with_tag(8, |w| w.write_int32(*&self.x_size))?; }
        if self.y_size != 0i32 { w.write_with_tag(16, |w| w.write_int32(*&self.y_size))?; }
        if self.color_space != GifFrameUpdate::mod_GIFMessage::ColorSpace::BIT_24_RGB { w.write_with_tag(24, |w| w.write_enum(*&self.color_space as i32))?; }
        if self.message_size != 0i32 { w.write_with_tag(32, |w| w.write_int32(*&self.message_size))?; }
        Ok(())
    }
}

pub mod mod_GIFMessage {


#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum ColorSpace {
    BIT_24_RGB = 0,
    BIT_16_RGB = 1,
    BIT_48_RGB = 2,
    BIT_8_RGB = 3,
}

impl Default for ColorSpace {
    fn default() -> Self {
        ColorSpace::BIT_24_RGB
    }
}

impl From<i32> for ColorSpace {
    fn from(i: i32) -> Self {
        match i {
            0 => ColorSpace::BIT_24_RGB,
            1 => ColorSpace::BIT_16_RGB,
            2 => ColorSpace::BIT_48_RGB,
            3 => ColorSpace::BIT_8_RGB,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for ColorSpace {
    fn from(s: &'a str) -> Self {
        match s {
            "BIT_24_RGB" => ColorSpace::BIT_24_RGB,
            "BIT_16_RGB" => ColorSpace::BIT_16_RGB,
            "BIT_48_RGB" => ColorSpace::BIT_48_RGB,
            "BIT_8_RGB" => ColorSpace::BIT_8_RGB,
            _ => Self::default(),
        }
    }
}

}

