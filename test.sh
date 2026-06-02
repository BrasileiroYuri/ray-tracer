for f in scenes/xmls/*.xml; do
  ./build/rt3 "$f"
done

for f in *{.ppm,.png}; do
  xdg-open "$f"
done
