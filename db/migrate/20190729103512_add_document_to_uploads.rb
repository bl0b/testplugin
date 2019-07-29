class AddDocumentToUploads < ActiveRecord::Migration[5.2]
  def change
    add_column :uploads, :uploaded_document, :string
  end
end
