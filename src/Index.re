/* From https://bucklescript.github.io/docs/en/interop-with-js-build-systems */
%raw
{|import './styles.css'|};

ReactDOMRe.renderToElementWithId(
  <>
    <TypedText
      preTypeDelay=70
      typeDelay=100
      preEraseDelay=2000
      eraseDelay=70
      texts=["student", "developer", "hacker"]
    />
    <Caret interval=500 />
  </>,
  "app",
);