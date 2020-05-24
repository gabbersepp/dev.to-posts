describe('Test', () => {
  beforeEach(() => {
    cy.task('resetCRI').visit('./../index.html')
  })

  it("file dialog open fails", () => {
    cy.get("input").click();
  })

  it('file dialog open succeeds', () => {
    const sizeContainer = window.frames["parent"].document.querySelector(".size-container");
    const cypressAppFrameContainerRect = sizeContainer.getBoundingClientRect();
    const marginLeft = parseFloat(getComputedStyle(sizeContainer).marginLeft);

    // pixel values passed to chrome debugger protocol must be integer
    const addX = Math.ceil(cypressAppFrameContainerRect.x + marginLeft);
    const addY = Math.ceil(cypressAppFrameContainerRect.y);

    cy.get("input").first().then($element => {
      const element = $element[0];
      element.scrollIntoView();
      var rect = element.getBoundingClientRect();
      cy.task("nativeClick", {x: parseInt(rect.x) + addX, y: parseInt(rect.y) + addY });
    })
  })
})
