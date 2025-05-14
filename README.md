# 1.62.25.05.13
- Add `FbMetadbHandleList` `SaveAs`. Saves using native `.fpl`
format so you should use that as the file extension. The
parent folder must already exist.

- Add `fb.ShowPictureViewer(image_path)`. This uses the image
viewer built in to `foobar2000`.

# 1.62.25.05.10
- `foobar2000` `2.25` preview has changed the behaviour of `FbMetadbHandle` `RawPath` if you
have a portable install and music files on the same drive. Any code that checks `startsWith("file://")`
will fail because the `RawPath` now starts with `file-relative://`. This release restores the old
behaviour.

# 1.62.25.05.05

- Requires at least `foobar2000` `2.0` because new methods from the `SDK` are being used.

- Add `fb.GetAudioChunk` / `FbAudioChunk` interface. See `vu meter` sample.

- Add `utils.GetClipboardText` / `utils.SetClipboardText`.

- Add `plman.GetGUID` / `plman.FindByGUID`.

- Fix `utils.ColourPicker` bugs

- Fix `utils.IsFile` / `utils.IsDirectory` bugs.

- Update various samples fixed by `regor`.

- The `on_library_items_changed` callback now has a secondary `fromhook` argument so you can ignore updates that are not tag edits but database updates from components like `foo_playcount`.

```js
function on_library_items_changed(handles, fromhook) {
    if (fromhook)
        return;
   // react to actual file tag changes here
}
```

- Various improvements to handle list iteration methods.

- Various other bug fixes.
